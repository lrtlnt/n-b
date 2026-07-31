#include "RunDialog.h"
#include "../core/Graphics.h"
#include "../core/Event.h"
#include <cstdlib>

RunDialog::RunDialog(Application* app)
    : Widget(Rect(0, 0, 400, 180)), m_app(app) {
    m_okButton = std::make_unique<Button>(
        Rect(m_rect.w - 260, m_rect.h - 40, 75, 25),
        "确定"
    );
    m_okButton->setOnClick([this]() {
        // 简单执行命令，Windows下用system，Android下暂时提示
#ifdef _WIN32
        if (!m_input.empty()) {
            std::string cmd = "start " + m_input;
            system(cmd.c_str());
        }
#endif
        for (auto& win : m_app->m_windows) {
            if (win->getContent() == this) {
                win->close();
                break;
            }
        }
    });
    
    m_cancelButton = std::make_unique<Button>(
        Rect(m_rect.w - 175, m_rect.h - 40, 75, 25),
        "取消"
    );
    m_cancelButton->setOnClick([this]() {
        for (auto& win : m_app->m_windows) {
            if (win->getContent() == this) {
                win->close();
                break;
            }
        }
    });
    
    m_browseButton = std::make_unique<Button>(
        Rect(m_rect.w - 90, m_rect.h - 40, 75, 25),
        "浏览..."
    );
    
    addChild(m_okButton.get());
    addChild(m_cancelButton.get());
    addChild(m_browseButton.get());
}

RunDialog::~RunDialog() = default;

void RunDialog::render() {
    if (!m_visible) return;
    
    Graphics::drawRect(m_rect, XPColors::WindowBackground);
    
    Graphics::drawText("请输入程序、文件夹、文档或 Internet 资源的名称，Windows 将为您打开它。",
                      m_rect.x + 15, m_rect.y + 20, XPColors::TextBlack, 12);
    Graphics::drawText("打开(O):", m_rect.x + 15, m_rect.y + 70, XPColors::TextBlack, 12);
    
    // 输入框
    Rect inputRect(m_rect.x + 70, m_rect.y + 65, m_rect.w - 90, 24);
    Graphics::drawRect(inputRect, {255, 255, 255, 255});
    Graphics::drawRect(inputRect, {128, 128, 128, 255}, false);
    Graphics::drawText(m_input, inputRect.x + 4, inputRect.y + 4, XPColors::TextBlack, 12);
    
    // 光标
    if (SDL_GetTicks() % 1000 < 500) {
        TTF_Font* font = Graphics::getFont(12);
        i32 w = 0;
        TTF_SizeText(font, m_input.c_str(), &w, nullptr);
        Graphics::drawLine(inputRect.x + 4 + w, inputRect.y + 4, inputRect.x + 4 + w, inputRect.y + 20, XPColors::TextBlack);
    }
    
    Widget::render();
}

void RunDialog::handleEvent(const Event& e) {
    Widget::handleEvent(e);
    
    if (e.type == EventType::TextInput) {
        m_input += e.text;
    } else if (e.type == EventType::KeyDown) {
        if (e.key == SDLK_BACKSPACE && !m_input.empty()) {
            m_input.pop_back();
        } else if (e.key == SDLK_RETURN) {
            m_okButton->setOnClick();
        } else if (e.key == SDLK_ESCAPE) {
            m_cancelButton->setOnClick();
        }
    }
}
