#include "Desktop.h"
#include "../core/Graphics.h"
#include "../core/Event.h"
#include "../core/FileSystem.h"
#include "../core/Application.h"

Desktop::Desktop(i32 width, i32 height, i32 taskBarHeight)
    : Widget(Rect(0, 0, width, height - taskBarHeight)), 
      m_draggedIcon(nullptr), m_dragging(false), m_taskBarHeight(taskBarHeight) {
    refreshIcons();
}

Desktop::~Desktop() = default;

void Desktop::refreshIcons() {
    m_icons.clear();
    
    // 添加内置快捷方式
    DesktopIcon myComputer;
    myComputer.name = "我的电脑";
    myComputer.path = "::{20D04FE0-3AEA-1069-A2D8-08002B30309D}"; // 特殊路径表示我的电脑
    myComputer.type = FileType::Drive;
    myComputer.position = Point(20, 20);
    myComputer.selected = false;
    myComputer.hovered = false;
    m_icons.push_back(myComputer);
    
    DesktopIcon myDocs;
    myDocs.name = "我的文档";
    myDocs.path = FileSystem::getDesktopPath() + "/../Documents";
    myDocs.type = FileType::Directory;
    myDocs.position = Point(20, 100);
    myDocs.selected = false;
    myDocs.hovered = false;
    m_icons.push_back(myDocs);
    
    DesktopIcon recycleBin;
    recycleBin.name = "回收站";
    recycleBin.path = "::{645FF040-5081-101B-9F08-00AA002F954E}";
    recycleBin.type = FileType::Directory;
    recycleBin.position = Point(20, 180);
    recycleBin.selected = false;
    recycleBin.hovered = false;
    m_icons.push_back(recycleBin);
    
    // 添加桌面文件
    std::string desktopPath = FileSystem::getDesktopPath();
    auto files = FileSystem::listDirectory(desktopPath);
    
    i32 col = 0, row = 3;
    for (auto& file : files) {
        // 跳过隐藏文件
        if (file.name[0] == '.') continue;
        
        DesktopIcon icon;
        icon.name = file.name;
        icon.path = file.path;
        icon.type = file.type;
        icon.position = Point(20 + col * ICON_SPACING, 20 + row * ICON_SPACING);
        icon.selected = false;
        icon.hovered = false;
        m_icons.push_back(icon);
        
        col++;
        if (col >= 8) {
            col = 0;
            row++;
        }
    }
}

void Desktop::render() {
    if (!m_visible) return;
    
    // 绘制Bliss壁纸
    Graphics::drawBlissWallpaper(m_rect.w, m_rect.h + m_taskBarHeight);
    
    // 绘制所有图标
    for (auto& icon : m_icons) {
        drawIcon(icon);
    }
    
    // 绘制右键菜单
    if (m_contextMenu && m_contextMenu->isVisible()) {
        m_contextMenu->render();
    }
}

void Desktop::drawIcon(const DesktopIcon& icon) {
    Rect iconRect(icon.position.x, icon.position.y, ICON_SIZE, ICON_SIZE);
    
    // 选中背景
    if (icon.selected) {
        Graphics::drawRect(Rect(icon.position.x - 4, icon.position.y - 4, ICON_SIZE + 8, ICON_SIZE + 24), 
                          {49, 106, 197, 100});
    }
    
    // 悬停效果
    if (icon.hovered && !icon.selected) {
        Graphics::drawRect(Rect(icon.position.x - 2, icon.position.y - 2, ICON_SIZE + 4, ICON_SIZE + 20),
                          {255, 255, 255, 50});
    }
    
    // 绘制图标（简单的图形表示，不用外部图片）
    if (icon.type == FileType::Drive) {
        // 我的电脑/驱动器图标：灰色矩形加绿色小条
        Graphics::drawRect(Rect(icon.position.x + 8, icon.position.y + 12, 32, 24), {200, 200, 200, 255});
        Graphics::drawRect(Rect(icon.position.x + 8, icon.position.y + 8, 32, 6), {180, 180, 180, 255});
        Graphics::drawRect(Rect(icon.position.x + 12, icon.position.y + 16, 4, 16), {0, 200, 0, 255});
    } else if (icon.type == FileType::Directory) {
        // 文件夹图标：黄色文件夹
        Graphics::drawRect(Rect(icon.position.x + 6, icon.position.y + 16, 36, 24), {255, 220, 100, 255});
        Graphics::drawRect(Rect(icon.position.x + 6, icon.position.y + 12, 16, 6), {255, 220, 100, 255});
        Graphics::drawLine(icon.position.x + 6, icon.position.y + 16, icon.position.x + 42, icon.position.y + 16, {220, 180, 60, 255});
    } else {
        // 文件图标：白色文档加折角
        Graphics::drawRect(Rect(icon.position.x + 10, icon.position.y + 8, 28, 32), {255, 255, 255, 255});
        Graphics::drawLine(icon.position.x + 10, icon.position.y + 8, icon.position.x + 38, icon.position.y + 40, {200, 200, 200, 255});
        Graphics::drawLine(icon.position.x + 30, icon.position.y + 8, icon.position.x + 38, icon.position.y + 16, {200, 200, 200, 255});
        Graphics::drawLine(icon.position.x + 30, icon.position.y + 8, icon.position.x + 30, icon.position.y + 16, {200, 200, 200, 255});
        Graphics::drawLine(icon.position.x + 30, icon.position.y + 16, icon.position.x + 38, icon.position.y + 16, {200, 200, 200, 255});
    }
    
    // 绘制图标文字
    SDL_Color textColor = icon.selected ? XPColors::SelectionText : XPColors::TextWhite;
    // 文字阴影
    Graphics::drawText(icon.name, icon.position.x + ICON_SIZE/2 + 1, icon.position.y + ICON_SIZE + 4 + 1, 
                      {0, 0, 0, 200}, 11, true);
    Graphics::drawText(icon.name, icon.position.x + ICON_SIZE/2, icon.position.y + ICON_SIZE + 4, 
                      textColor, 11, true);
}

void Desktop::handleEvent(const Event& e) {
    if (!m_visible) return;
    
    // 处理右键菜单事件
    if (m_contextMenu && m_contextMenu->isVisible()) {
        m_contextMenu->handleEvent(e);
        if (!m_contextMenu->isVisible()) {
            m_contextMenu.reset();
        }
        return;
    }
    
    // 处理图标拖动
    if (m_dragging && m_draggedIcon) {
        if (e.type == EventType::MouseMove) {
            m_draggedIcon->position.x = e.position.x - m_dragOffset.x;
            m_draggedIcon->position.y = e.position.y - m_dragOffset.y;
        } else if (e.type == EventType::MouseUp) {
            m_dragging = false;
            m_draggedIcon = nullptr;
        }
        return;
    }
    
    // 检查图标事件
    bool clickedIcon = false;
    DesktopIcon* clickedIconPtr = nullptr;
    
    for (auto& icon : m_icons) {
        Rect iconHitRect(icon.position.x - 4, icon.position.y - 4, ICON_SIZE + 8, ICON_SIZE + 24);
        icon.hovered = iconHitRect.contains(e.position);
        
        if (iconHitRect.contains(e.position)) {
            clickedIcon = true;
            clickedIconPtr = &icon;
        }
    }
    
    switch (e.type) {
        case EventType::MouseDown:
            if (clickedIcon && e.button == MouseButton::Left) {
                // 选中图标
                for (auto& icon : m_icons) {
                    icon.selected = false;
                }
                clickedIconPtr->selected = true;
                
                // 开始拖动
                m_dragging = true;
                m_draggedIcon = clickedIconPtr;
                m_dragOffset.x = e.position.x - clickedIconPtr->position.x;
                m_dragOffset.y = e.position.y - clickedIconPtr->position.y;
            } else {
                // 点击空白处取消选中
                for (auto& icon : m_icons) {
                    icon.selected = false;
                }
            }
            break;
            
        case EventType::MouseClick:
            if (clickedIcon) {
                // 单击选中已经在MouseDown处理
            }
            break;
            
        case EventType::MouseDoubleClick:
            if (clickedIcon) {
                // 双击打开图标
                extern Application* g_app;
                if (clickedIconPtr->type == FileType::Drive || clickedIconPtr->type == FileType::Directory) {
                    // 打开文件管理器（简单用记事本？不，应该打开文件浏览器，不过先简单处理，或者打开对应应用）
                    if (clickedIconPtr->path.find("20D04FE0") != std::string::npos) {
                        // 我的电脑，暂时打开根目录
                        g_app->openFileBrowser("/");
                    } else {
                        g_app->openFileBrowser(clickedIconPtr->path);
                    }
                } else {
                    // 文件用对应程序打开
                    std::string ext = clickedIconPtr->path.substr(clickedIconPtr->path.find_last_of('.'));
                    if (ext == ".txt" || ext == ".log" || ext == ".ini") {
                        g_app->openNotepad(clickedIconPtr->path);
                    } else if (ext == ".bmp" || ext == ".png" || ext == ".jpg") {
                        g_app->openImageViewer(clickedIconPtr->path);
                    }
                }
            }
            break;
            
        case EventType::RightClick:
            if (clickedIcon) {
                showIconMenu(clickedIconPtr, e.position);
            } else {
                showDesktopMenu(e.position);
            }
            break;
            
        default:
            break;
    }
}

void Desktop::update(u32 deltaTime) {
    if (m_contextMenu) {
        m_contextMenu->update(deltaTime);
    }
}

void Desktop::showDesktopMenu(const Point& pos) {
    std::vector<MenuItem> items;
    items.emplace_back("查看", [](){});
    items.emplace_back("排列图标", [](){});
    items.emplace_back("刷新", [this]() {
        refreshIcons();
    });
    items.emplace_back();
    items.back().separator = true;
    items.emplace_back("新建", [](){});
    items.emplace_back();
    items.back().separator = true;
    items.emplace_back("属性", [](){});
    
    m_contextMenu = std::make_unique<ContextMenu>(pos, items);
    m_contextMenu->show();
}

void Desktop::showIconMenu(DesktopIcon* icon, const Point& pos) {
    std::vector<MenuItem> items;
    items.emplace_back("打开", [this, icon]() {
        extern Application* g_app;
        if (icon->type == FileType::Directory || icon->type == FileType::Drive) {
            g_app->openFileBrowser(icon->path);
        } else {
            g_app->openNotepad(icon->path);
        }
    });
    items.emplace_back();
    items.back().separator = true;
    items.emplace_back("剪切", [icon]() {
        FileSystem::setClipboardFiles({icon->path}, true);
    });
    items.emplace_back("复制", [icon]() {
        FileSystem::setClipboardFiles({icon->path}, false);
    });
    items.emplace_back("删除", [this, icon]() {
        FileSystem::deleteFile(icon->path);
        refreshIcons();
    });
    items.emplace_back("重命名", [](){});
    items.emplace_back();
    items.back().separator = true;
    items.emplace_back("属性", [](){});
    
    m_contextMenu = std::make_unique<ContextMenu>(pos, items);
    m_contextMenu->show();
}
