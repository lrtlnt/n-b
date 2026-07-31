#include "Button.h"
#include "../core/Graphics.h"

Button::Button(const Rect& rect, const std::string& text, ButtonType type)
    : Widget(rect, text), m_type(type) {
}

void Button::render() {
    if (!m_visible) return;
    
    switch (m_type) {
        case ButtonType::Normal:
            Graphics::draw3DButton(m_rect, m_pressed, m_hovered);
            Graphics::drawTextInRect(m_text, m_rect, XPColors::TextBlack, 12);
            break;
            
        case ButtonType::TitleBarMin:
        case ButtonType::TitleBarMax:
        case ButtonType::TitleBarClose: {
            // 标题栏按钮背景
            if (m_hovered) {
                if (m_type == ButtonType::TitleBarClose) {
                    Graphics::drawRect(m_rect, XPColors::CloseButtonHover);
                } else {
                    Graphics::drawRect(m_rect, {60, 120, 200, 255});
                }
            }
            
            // 绘制按钮图标
            SDL_Color iconColor = m_hovered ? XPColors::TextWhite : XPColors::TextWhite;
            i32 cx = m_rect.x + m_rect.w / 2;
            i32 cy = m_rect.y + m_rect.h / 2;
            
            if (m_type == ButtonType::TitleBarMin) {
                // 最小化：横线
                Graphics::drawLine(cx - 6, cy + 4, cx + 6, cy + 4, iconColor);
            } else if (m_type == ButtonType::TitleBarMax) {
                // 最大化：方框
                Graphics::drawLine(cx - 5, cy - 4, cx + 5, cy - 4, iconColor);
                Graphics::drawLine(cx - 5, cy - 4, cx - 5, cy + 4, iconColor);
                Graphics::drawLine(cx + 5, cy - 4, cx + 5, cy + 4, iconColor);
                Graphics::drawLine(cx - 5, cy + 4, cx + 5, cy + 4, iconColor);
            } else if (m_type == ButtonType::TitleBarClose) {
                // 关闭：X
                Graphics::drawLine(cx - 5, cy - 4, cx + 5, cy + 4, iconColor);
                Graphics::drawLine(cx + 5, cy - 4, cx - 5, cy + 4, iconColor);
            }
            break;
        }
            
        case ButtonType::StartButton: {
            // 绿色开始按钮，带圆角效果
            SDL_Color bgColor = m_hovered ? XPColors::StartButtonGreenHover : XPColors::StartButtonGreen;
            if (m_pressed) {
                bgColor.r -= 30;
                bgColor.g -= 30;
                bgColor.b -= 30;
            }
            Graphics::drawRect(m_rect, bgColor);
            
            // 3D边框
            Graphics::drawLine(m_rect.x, m_rect.y, m_rect.x + m_rect.w - 1, m_rect.y, {80, 220, 80, 255});
            Graphics::drawLine(m_rect.x, m_rect.y, m_rect.x, m_rect.y + m_rect.h - 1, {80, 220, 80, 255});
            Graphics::drawLine(m_rect.x, m_rect.y + m_rect.h - 1, m_rect.x + m_rect.w - 1, m_rect.y + m_rect.h - 1, {20, 100, 20, 255});
            Graphics::drawLine(m_rect.x + m_rect.w - 1, m_rect.y, m_rect.x + m_rect.w - 1, m_rect.y + m_rect.h - 1, {20, 100, 20, 255});
            
            // Windows徽标简单绘制（四个小方块）
            i32 logoX = m_rect.x + 8;
            i32 logoY = m_rect.y + 6;
            Graphics::drawRect(Rect(logoX, logoY, 6, 6), {255, 100, 100, 255});
            Graphics::drawRect(Rect(logoX + 7, logoY, 6, 6), {100, 255, 100, 255});
            Graphics::drawRect(Rect(logoX, logoY + 7, 6, 6), {100, 100, 255, 255});
            Graphics::drawRect(Rect(logoX + 7, logoY + 7, 6, 6), {255, 255, 100, 255});
            
            // "开始"文字
            Graphics::drawText("开始", m_rect.x + 30, m_rect.y + 7, XPColors::TextWhite, 14);
            break;
        }
    }
    
    Widget::render();
}

void Button::handleEvent(const Event& e) {
    Widget::handleEvent(e);
}

void Button::setButtonType(ButtonType type) {
    m_type = type;
}

ButtonType Button::getButtonType() const {
    return m_type;
}
