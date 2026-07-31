#pragma once

#include "Widget.h"
#include "ContextMenu.h"
#include "../core/FileSystem.h"
#include <vector>
#include <memory>

struct DesktopIcon {
    std::string name;
    std::string path;
    FileType type;
    Point position;
    bool selected;
    bool hovered;
};

class Desktop : public Widget {
public:
    Desktop(i32 width, i32 height, i32 taskBarHeight);
    ~Desktop() override;
    
    void render() override;
    void handleEvent(const Event& e) override;
    void update(u32 deltaTime) override;
    
    void refreshIcons();
    
private:
    void drawIcon(const DesktopIcon& icon);
    void showDesktopMenu(const Point& pos);
    void showIconMenu(DesktopIcon* icon, const Point& pos);
    
    std::vector<DesktopIcon> m_icons;
    std::unique_ptr<ContextMenu> m_contextMenu;
    DesktopIcon* m_draggedIcon;
    Point m_dragOffset;
    bool m_dragging;
    i32 m_taskBarHeight;
    
    static constexpr i32 ICON_SIZE = 48;
    static constexpr i32 ICON_SPACING = 80;
};
