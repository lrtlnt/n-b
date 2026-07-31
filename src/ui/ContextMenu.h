#pragma once

#include "Widget.h"
#include <vector>
#include <functional>

struct MenuItem {
    std::string text;
    std::function<void()> callback;
    bool separator;
    bool disabled;
    
    MenuItem() : separator(false), disabled(false) {}
    MenuItem(const std::string& text, std::function<void()> callback = nullptr)
        : text(text), callback(callback), separator(false), disabled(false) {}
};

class ContextMenu : public Widget {
public:
    ContextMenu(const Point& position, const std::vector<MenuItem>& items, bool alignBottom = false);
    
    void render() override;
    void handleEvent(const Event& e) override;
    
    void show();
    void hide();
    bool isVisible() const;
    
private:
    std::vector<MenuItem> m_items;
    i32 m_hoveredIndex;
    bool m_alignBottom; // 是否底部对齐（用于开始菜单向上弹出）
    static constexpr i32 ITEM_HEIGHT = 24;
    static constexpr i32 MENU_WIDTH = 200;
};
