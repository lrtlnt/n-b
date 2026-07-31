#include "About.h"
#include "../core/Graphics.h"
#include "../core/Event.h"

About::About(Application* app, const std::string& title, const std::string& text)
    : Widget(Rect(0, 0, 360, 240)), m_app(app), m_text(text) {
    m_okButton = std::make_unique<Button>(
        Rect(m_rect.w/2 - 40, m_rect.h - 40, 80, 28),
        "确定"
    );
    m_okButton->setOnClick([this]() {
        // 关闭窗口，通过父窗口？简单处理，发送关闭事件
        SDL_Event sdlEvent;
        sdlEvent.type = SDL_QUIT; // 不对，应该关闭当前窗口，简单起见，让Application处理，或者直接标记？
        // 简单实现：找到包含这个控件的窗口并关闭
        for (auto& win : m_app->m_windows) {
            if (win->getContent() == this) {
                win->close();
                break;
            }
        }
    });
    addChild(m_okButton.get());
}

About::~About() = default;

void About::render() {
    if (!m_visible) return;
    
    Graphics::drawRect(m_rect, XPColors::WindowBackground);
    
    // 绘制文本，按行分割
    std::istringstream stream(m_text);
    std::string line;
    i32 y = m_rect.y + 30;
    while (std::getline(stream, line)) {
        Graphics::drawText(line, m_rect.x + 30, y, XPColors::TextBlack, 14);
        y += 22;
    }
    
    // XP图标简单绘制
    Graphics::drawRect(Rect(m_rect.x + 20, m_rect.y + 20, 48, 48), {0, 120, 200, 255});
    // 四个色块
    Graphics::drawRect(Rect(m_rect.x + 25, m_rect.y + 25, 18, 18), {255, 100, 100, 255});
    Graphics::drawRect(Rect(m_rect.x + 45, m_rect.y + 25, 18, 18), {100, 255, 100, 255});
    Graphics::drawRect(Rect(m_rect.x + 25, m_rect.y + 45, 18, 18), {100, 100, 255, 255});
    Graphics::drawRect(Rect(m_rect.x + 45, m_rect.y + 45, 18, 18), {255, 255, 100, 255});
    
    Widget::render();
}

void About::handleEvent(const Event& e) {
    Widget::handleEvent(e);
}
