#include "TaskBar.h"
#include "../core/Graphics.h"
#include "../core/Event.h"
#include "../core/Window.h"
#include <ctime>
#include <sstream>
#include <iomanip>

TaskBar::TaskBar(i32 screenWidth, i32 screenHeight)
    : Widget(Rect(0, screenHeight - TASKBAR_HEIGHT, screenWidth, TASKBAR_HEIGHT)),
      m_height(TASKBAR_HEIGHT), m_timeUpdateTimer(0) {
    
    // 创建开始按钮
    m_startButton = std::make_unique<Button>(
        Rect(4, 2, 100, TASKBAR_HEIGHT - 4),
        "",
        ButtonType::StartButton
    );
    m_startButton->setOnClick([this]() {
        showStartMenu();
    });
    addChild(m_startButton.get());
    
    // 初始化时间
    updateClock();
}

TaskBar::~TaskBar() = default;

void TaskBar::render() {
    if (!m_visible) return;
    
    // 任务栏蓝色渐变
    Graphics::drawGradientRect(m_rect, XPColors::TaskBarStart, XPColors::TaskBarEnd, true);
    
    // 顶部边框
    Graphics::drawLine(m_rect.x, m_rect.y, m_rect.x + m_rect.w, m_rect.y, {80, 150, 220, 255});
    
    // 渲染子控件（开始按钮、窗口按钮）
    Widget::render();
    
    // 系统托盘区域
    Rect trayRect(m_rect.x + m_rect.w - 100, m_rect.y + 2, 96, m_rect.h - 4);
    Graphics::drawGradientRect(trayRect, {20, 80, 160, 255}, {10, 60, 140, 255}, true);
    
    // 时钟
    Graphics::drawText(m_currentTime, trayRect.x + 10, trayRect.y + 8, XPColors::TextWhite, 14);
}

void TaskBar::handleEvent(const Event& e) {
    Widget::handleEvent(e);
    
    // 处理开始菜单事件
    if (m_startMenu && m_startMenu->isVisible()) {
        m_startMenu->handleEvent(e);
    }
}

void TaskBar::update(u32 deltaTime) {
    Widget::update(deltaTime);
    
    // 每秒更新时钟
    m_timeUpdateTimer += deltaTime;
    if (m_timeUpdateTimer >= 1000) {
        updateClock();
        m_timeUpdateTimer = 0;
    }
    
    if (m_startMenu) {
        m_startMenu->update(deltaTime);
    }
}

void TaskBar::updateClock() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    
    std::stringstream ss;
    ss << std::put_time(localTime, "%H:%M");
    m_currentTime = ss.str();
}

void TaskBar::addWindow(Window* window) {
    // 创建窗口按钮
    i32 buttonX = 110 + m_windowButtons.size() * 160;
    auto button = std::make_unique<Button>(
        Rect(buttonX, 4, 150, m_height - 8),
        window->getTitle()
    );
    
    Window* winPtr = window;
    button->setOnClick([winPtr]() {
        if (winPtr->isMinimized()) {
            winPtr->restore();
        } else if (winPtr->isActive()) {
            winPtr->minimize();
        } else {
            winPtr->activate();
        }
    });
    
    m_windowButtons.emplace_back(window, std::move(button));
    addChild(m_windowButtons.back().second.get());
}

void TaskBar::removeWindow(Window* window) {
    for (auto it = m_windowButtons.begin(); it != m_windowButtons.end(); ++it) {
        if (it->first == window) {
            removeChild(it->second.get());
            m_windowButtons.erase(it);
            break;
        }
    }
    
    // 重新排列窗口按钮
    for (i32 i = 0; i < m_windowButtons.size(); i++) {
        i32 buttonX = 110 + i * 160;
        m_windowButtons[i].second->setRect(Rect(buttonX, 4, 150, m_height - 8));
    }
}

void TaskBar::updateWindowButton(Window* window) {
    for (auto& pair : m_windowButtons) {
        if (pair.first == window) {
            pair.second->setText(window->getTitle());
            // 活动窗口按钮按下状态
            pair.second->setPressed(window->isActive() && !window->isMinimized());
            break;
        }
    }
}

i32 TaskBar::getHeight() const {
    return m_height;
}

void TaskBar::showStartMenu() {
    // 创建开始菜单
    std::vector<MenuItem> items;
    items.emplace_back("📝 记事本", []() {
        // 打开记事本
        extern Application* g_app;
        g_app->openNotepad();
    });
    items.emplace_back("🔢 计算器", []() {
        extern Application* g_app;
        g_app->openCalculator();
    });
    items.emplace_back("🎨 画图", []() {
        extern Application* g_app;
        g_app->openPaint();
    });
    items.emplace_back("🖼️ 图片查看器", []() {
        extern Application* g_app;
        g_app->openImageViewer();
    });
    items.emplace_back("🌐 浏览器", []() {
        extern Application* g_app;
        g_app->openBrowser();
    });
    items.emplace_back();
    items.back().separator = true;
    items.emplace_back("▶️ 运行...", []() {
        extern Application* g_app;
        g_app->openRunDialog();
    });
    items.emplace_back();
    items.back().separator = true;
    items.emplace_back("ℹ️ 关于 Windows XP", []() {
        extern Application* g_app;
        g_app->openAbout();
    });
    items.emplace_back("❌ 关机", []() {
        extern Application* g_app;
        g_app->quit();
    });
    
    // 菜单位置：开始按钮上方，底部对齐任务栏
    Point menuPos(m_startButton->getRect().x, m_rect.y);
    m_startMenu = std::make_unique<ContextMenu>(menuPos, items, true);
    m_startMenu->show();
}
