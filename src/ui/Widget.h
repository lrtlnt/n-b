#pragma once

#include "../core/Types.h"
#include <vector>
#include <string>
#include <functional>

class Widget {
public:
    Widget(const Rect& rect, const std::string& text = "");
    virtual ~Widget() = default;
    
    virtual void render();
    virtual void handleEvent(const Event& e);
    virtual void update(u32 deltaTime);
    
    // 属性
    void setRect(const Rect& rect);
    Rect getRect() const;
    void setText(const std::string& text);
    std::string getText() const;
    void setVisible(bool visible);
    bool isVisible() const;
    void setEnabled(bool enabled);
    bool isEnabled() const;
    void setHovered(bool hovered);
    bool isHovered() const;
    void setPressed(bool pressed);
    bool isPressed() const;
    
    // 子控件管理
    void addChild(Widget* child);
    void removeChild(Widget* child);
    const std::vector<Widget*>& getChildren() const;
    
    // 事件回调
    void setOnClick(std::function<void()> callback);
    void setOnRightClick(std::function<void(const Point&)> callback);
    
protected:
    Rect m_rect;
    std::string m_text;
    bool m_visible;
    bool m_enabled;
    bool m_hovered;
    bool m_pressed;
    Widget* m_parent;
    std::vector<Widget*> m_children;
    
    std::function<void()> m_onClick;
    std::function<void(const Point&)> m_onRightClick;
};
