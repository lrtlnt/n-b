#include "Window.h"
#include "Graphics.h"
#include "Event.h"
#include "Application.h"

Window::Window(Application* app, const std::string& title, const Rect& rect)
    : Widget(rect), m_app(app), m_title(title), m_state(WindowState::Normal),
      m_dragging(false), m_isActive(false), m_content(nullptr) {
    
    m_restoreRect = rect;
    
    // 创建标题栏按钮
    i32 btnY = 4;
    i32 btnSize = 22;
    m_closeButton = std::make_unique<Button>(
        Rect(rect.w - btnSize - 4, btnY, btnSize, btnSize),
        "", ButtonType::TitleBarClose
    );
    m_closeButton->setOnClick([this]() { close(); });
    
    m_maxButton = std::make_unique<Button>(
        Rect(rect.w - btnSize*2 - 8, btnY, btnSize, btnSize),
        "", ButtonType::TitleBarMax
    );
    m_maxButton->setOnClick([this]() {
        if (m_state == WindowState::Maximized) {
            restore();
        } else {
            maximize();
        }
    });
    
    m_minButton = std::make_unique<Button>(
        Rect(rect.w - btnSize*3 - 12, btnY, btnSize, btnSize),
        "", ButtonType::TitleBarMin
    );
    m_minButton->setOnClick([this]() { minimize(); });
    
    addChild(m_minButton.get());
    addChild(m_maxButton.get());
    addChild(m_closeButton.get());
}

Window::~Window() {
    if (m_content) {
        delete m_content;
    }
}

void Window::render() {
    if (!m_visible || m_state == WindowState::Minimized) return;
    
    // 窗口边框
    Graphics::drawRect(m_rect, XPColors::WindowBorder, false);
    
    // 标题栏
    Rect titleBarRect(m_rect.x, m_rect.y, m_rect.w, TITLEBAR_HEIGHT);
    if (m_state == WindowState::Maximized) {
        titleBarRect.w = m_rect.w;
    }
    
    if (m_isActive) {
        Graphics::drawGradientRect(titleBarRect, XPColors::TitleBarActiveStart, XPColors::TitleBarActiveEnd, true);
        Graphics::drawText(m_title, titleBarRect.x + 8, titleBarRect.y + 7, XPColors::TextWhite, 14);
    } else {
        Graphics::drawGradientRect(titleBarRect, XPColors::TitleBarInactiveStart, XPColors::TitleBarInactiveEnd, true);
        Graphics::drawText(m_title, titleBarRect.x + 8, titleBarRect.y + 7, XPColors::TextGray, 14);
    }
    
    // 客户区背景
    Rect clientRect = getClientRect();
    Graphics::drawRect(clientRect, XPColors::WindowBackground);
    
    // 3D边框效果
    Graphics::drawLine(m_rect.x, m_rect.y, m_rect.x + m_rect.w - 1, m_rect.y, XPColors::ButtonHighlight);
    Graphics::drawLine(m_rect.x, m_rect.y, m_rect.x, m_rect.y + m_rect.h - 1, XPColors::ButtonHighlight);
    Graphics::drawLine(m_rect.x, m_rect.y + m_rect.h - 1, m_rect.x + m_rect.w - 1, m_rect.y + m_rect.h - 1, XPColors::ButtonDarkShadow);
    Graphics::drawLine(m_rect.x + m_rect.w - 1, m_rect.y, m_rect.x + m_rect.w - 1, m_rect.y + m_rect.h - 1, XPColors::ButtonDarkShadow);
    
    // 渲染子控件（标题栏按钮、内容）
    Widget::render();
    
    // 渲染客户区内容
    if (m_content) {
        m_content->render();
    }
}

void Window::handleEvent(const Event& e) {
    if (!m_visible || m_state == WindowState::Minimized) return;
    
    // 先让子控件处理
    Widget::handleEvent(e);
    
    // 标题栏拖动
    Rect titleBarRect(m_rect.x, m_rect.y, m_rect.w - 100, TITLEBAR_HEIGHT);
    if (e.type == EventType::MouseDown && titleBarRect.contains(e.position) && e.button == MouseButton::Left) {
        m_dragging = true;
        m_dragOffset.x = e.position.x - m_rect.x;
        m_dragOffset.y = e.position.y - m_rect.y;
        activate();
    }
    
    if (e.type == EventType::MouseUp && e.button == MouseButton::Left) {
        m_dragging = false;
    }
    
    if (e.type == EventType::MouseMove && m_dragging) {
        if (m_state == WindowState::Maximized) {
            // 最大化时拖动，还原窗口
            restore();
            m_dragOffset.x = m_rect.w / 2;
            m_dragOffset.y = TITLEBAR_HEIGHT / 2;
        }
        m_rect.x = e.position.x - m_dragOffset.x;
        m_rect.y = e.position.y - m_dragOffset.y;
        
        // 更新按钮位置
        i32 btnSize = 22;
        m_closeButton->setRect(Rect(m_rect.w - btnSize - 4, 4, btnSize, btnSize));
        m_maxButton->setRect(Rect(m_rect.w - btnSize*2 - 8, 4, btnSize, btnSize));
        m_minButton->setRect(Rect(m_rect.w - btnSize*3 - 12, 4, btnSize, btnSize));
        
        // 更新内容位置
        if (m_content) {
            m_content->setRect(getClientRect());
        }
    }
    
    // 双击标题栏最大化/还原
    if (e.type == EventType::MouseDoubleClick && titleBarRect.contains(e.position)) {
        if (m_state == WindowState::Maximized) {
            restore();
        } else {
            maximize();
        }
    }
    
    // 点击窗口激活
    if (e.type == EventType::MouseDown && m_rect.contains(e.position)) {
        activate();
    }
    
    // 传递事件给内容
    if (m_content) {
        // 转换坐标到客户区
        Event clientEvent = e;
        Rect clientRect = getClientRect();
        clientEvent.position.x -= clientRect.x;
        clientEvent.position.y -= clientRect.y;
        m_content->handleEvent(clientEvent);
    }
}

void Window::update(u32 deltaTime) {
    Widget::update(deltaTime);
    if (m_content) {
        m_content->update(deltaTime);
    }
}

void Window::setTitle(const std::string& title) {
    m_title = title;
    m_app->updateTaskBar();
}

std::string Window::getTitle() const {
    return m_title;
}

void Window::activate() {
    m_app->activateWindow(this);
}

void Window::deactivate() {
    m_isActive = false;
}

bool Window::isActive() const {
    return m_isActive;
}

void Window::minimize() {
    if (m_state == WindowState::Minimized) return;
    if (m_state == WindowState::Normal) {
        m_restoreRect = m_rect;
    }
    m_state = WindowState::Minimized;
    m_visible = false;
    m_app->updateTaskBar();
}

void Window::maximize() {
    if (m_state == WindowState::Maximized) return;
    if (m_state == WindowState::Normal) {
        m_restoreRect = m_rect;
    }
    m_state = WindowState::Maximized;
    i32 taskBarHeight = m_app->getTaskBarHeight();
    m_rect = Rect(0, 0, m_app->getScreenWidth(), m_app->getScreenHeight() - taskBarHeight);
    
    // 更新按钮位置
    i32 btnSize = 22;
    m_closeButton->setRect(Rect(m_rect.w - btnSize - 4, 4, btnSize, btnSize));
    m_maxButton->setRect(Rect(m_rect.w - btnSize*2 - 8, 4, btnSize, btnSize));
    m_minButton->setRect(Rect(m_rect.w - btnSize*3 - 12, 4, btnSize, btnSize));
    
    if (m_content) {
        m_content->setRect(getClientRect());
    }
    m_app->updateTaskBar();
}

void Window::restore() {
    if (m_state == WindowState::Normal) return;
    m_state = WindowState::Normal;
    m_rect = m_restoreRect;
    m_visible = true;
    
    // 更新按钮位置
    i32 btnSize = 22;
    m_closeButton->setRect(Rect(m_rect.w - btnSize - 4, 4, btnSize, btnSize));
    m_maxButton->setRect(Rect(m_rect.w - btnSize*2 - 8, 4, btnSize, btnSize));
    m_minButton->setRect(Rect(m_rect.w - btnSize*3 - 12, 4, btnSize, btnSize));
    
    if (m_content) {
        m_content->setRect(getClientRect());
    }
    m_app->updateTaskBar();
}

void Window::close() {
    m_app->closeWindow(this);
}

bool Window::isMinimized() const {
    return m_state == WindowState::Minimized;
}

bool Window::isMaximized() const {
    return m_state == WindowState::Maximized;
}

WindowState Window::getState() const {
    return m_state;
}

Rect Window::getClientRect() const {
    return Rect(
        m_rect.x + BORDER_WIDTH,
        m_rect.y + TITLEBAR_HEIGHT,
        m_rect.w - BORDER_WIDTH * 2,
        m_rect.h - TITLEBAR_HEIGHT - BORDER_WIDTH
    );
}

void Window::setContent(Widget* content) {
    if (m_content) {
        removeChild(m_content);
        delete m_content;
    }
    m_content = content;
    m_content->setRect(getClientRect());
    addChild(m_content);
}
