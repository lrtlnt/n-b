#include "ContextMenu.h"
#include "../core/Graphics.h"
#include "../core/Event.h"

ContextMenu::ContextMenu(const Point& position, const std::vector<MenuItem>& items, bool alignBottom)
    : Widget(Rect(position.x, position.y, MENU_WIDTH, 0)), m_items(items), 
      m_hoveredIndex(-1), m_alignBottom(alignBottom) {
    // 计算菜单高度
    i32 height = 4; // 上下边距
    for (auto& item : m_items) {
        if (item.separator) {
            height += 8;
        } else {
            height += ITEM_HEIGHT;
        }
    }
    height += 4;
    
    m_rect.h = height;
    
    // 如果是底部对齐，调整y坐标，让菜单底部在position.y位置（向上弹出）
    if (m_alignBottom) {
        m_rect.y = position.y - height;
    }
    
    m_visible = false;
}

void ContextMenu::render() {
    if (!m_visible) return;
    
    // 菜单背景和边框
    Graphics::drawRect(m_rect, XPColors::MenuBackground);
    Graphics::drawRect(m_rect, XPColors::MenuBorder, false);
    
    i32 y = m_rect.y + 4;
    for (i32 i = 0; i < m_items.size(); i++) {
        auto& item = m_items[i];
        
        if (item.separator) {
            // 分隔线
            Graphics::drawLine(m_rect.x + 4, y + 3, m_rect.x + m_rect.w - 4, y + 3, XPColors::MenuSeparator);
            y += 8;
            continue;
        }
        
        Rect itemRect(m_rect.x + 2, y, m_rect.w - 4, ITEM_HEIGHT);
        
        // 选中项高亮
        if (i == m_hoveredIndex && !item.disabled) {
            Graphics::drawRect(itemRect, XPColors::SelectionBlue);
            Graphics::drawText(item.text, itemRect.x + 8, itemRect.y + 4, XPColors::SelectionText, 12);
        } else {
            SDL_Color textColor = item.disabled ? XPColors::TextGray : XPColors::TextBlack;
            Graphics::drawText(item.text, itemRect.x + 8, itemRect.y + 4, textColor, 12);
        }
        
        y += ITEM_HEIGHT;
    }
    
    Widget::render();
}

void ContextMenu::handleEvent(const Event& e) {
    if (!m_visible) return;
    
    // 点击菜单外关闭菜单
    if (e.type == EventType::MouseClick && !m_rect.contains(e.position)) {
        hide();
        return;
    }
    
    if (e.type == EventType::RightClick) {
        hide();
        return;
    }
    
    // 计算悬停项
    if (e.type == EventType::MouseMove) {
        m_hoveredIndex = -1;
        i32 y = m_rect.y + 4;
        for (i32 i = 0; i < m_items.size(); i++) {
            auto& item = m_items[i];
            if (item.separator) {
                y += 8;
                continue;
            }
            
            Rect itemRect(m_rect.x + 2, y, m_rect.w - 4, ITEM_HEIGHT);
            if (itemRect.contains(e.position)) {
                m_hoveredIndex = i;
                break;
            }
            y += ITEM_HEIGHT;
        }
    }
    
    // 点击菜单项
    if (e.type == EventType::MouseClick && m_rect.contains(e.position)) {
        i32 y = m_rect.y + 4;
        for (i32 i = 0; i < m_items.size(); i++) {
            auto& item = m_items[i];
            if (item.separator) {
                y += 8;
                continue;
            }
            
            Rect itemRect(m_rect.x + 2, y, m_rect.w - 4, ITEM_HEIGHT);
            if (itemRect.contains(e.position) && !item.disabled) {
                if (item.callback) {
                    item.callback();
                }
                hide();
                break;
            }
            y += ITEM_HEIGHT;
        }
    }
}

void ContextMenu::show() {
    m_visible = true;
    m_hoveredIndex = -1;
}

void ContextMenu::hide() {
    m_visible = false;
}

bool ContextMenu::isVisible() const {
    return m_visible;
}
