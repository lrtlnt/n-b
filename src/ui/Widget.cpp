#include "Widget.h"
#include "../core/Graphics.h"
#include "../core/Event.h"

Widget::Widget(const Rect& rect, const std::string& text)
    : m_rect(rect), m_text(text), m_visible(true), m_enabled(true), 
      m_hovered(false), m_pressed(false), m_parent(nullptr) {
}

void Widget::render() {
    if (!m_visible) return;
    
    // 渲染子控件
    for (auto* child : m_children) {
        child->render();
    }
}

void Widget::handleEvent(const Event& e) {
    if (!m_visible || !m_enabled) return;
    
    // 先传递给子控件（逆序，最上层的先处理）
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        (*it)->handleEvent(e);
    }
    
    switch (e.type) {
        case EventType::MouseMove:
            m_hovered = m_rect.contains(e.position);
            break;
            
        case EventType::MouseDown:
            if (m_rect.contains(e.position) && e.button == MouseButton::Left) {
                m_pressed = true;
            }
            break;
            
        case EventType::MouseUp:
            if (m_pressed && m_rect.contains(e.position) && e.button == MouseButton::Left) {
                m_pressed = false;
                if (m_onClick) {
                    m_onClick();
                }
            } else {
                m_pressed = false;
            }
            break;
            
        case EventType::RightClick:
            if (m_rect.contains(e.position) && m_onRightClick) {
                m_onRightClick(e.position);
            }
            break;
            
        default:
            break;
    }
}

void Widget::update(u32 deltaTime) {
    for (auto* child : m_children) {
        child->update(deltaTime);
    }
}

void Widget::setRect(const Rect& rect) {
    m_rect = rect;
}

Rect Widget::getRect() const {
    return m_rect;
}

void Widget::setText(const std::string& text) {
    m_text = text;
}

std::string Widget::getText() const {
    return m_text;
}

void Widget::setVisible(bool visible) {
    m_visible = visible;
}

bool Widget::isVisible() const {
    return m_visible;
}

void Widget::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool Widget::isEnabled() const {
    return m_enabled;
}

void Widget::setHovered(bool hovered) {
    m_hovered = hovered;
}

bool Widget::isHovered() const {
    return m_hovered;
}

void Widget::setPressed(bool pressed) {
    m_pressed = pressed;
}

bool Widget::isPressed() const {
    return m_pressed;
}

void Widget::addChild(Widget* child) {
    child->m_parent = this;
    m_children.push_back(child);
}

void Widget::removeChild(Widget* child) {
    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        if (*it == child) {
            m_children.erase(it);
            child->m_parent = nullptr;
            break;
        }
    }
}

const std::vector<Widget*>& Widget::getChildren() const {
    return m_children;
}

void Widget::setOnClick(std::function<void()> callback) {
    m_onClick = callback;
}

void Widget::setOnRightClick(std::function<void(const Point&)> callback) {
    m_onRightClick = callback;
}
