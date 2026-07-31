#include "FileBrowser.h"
#include "../core/Graphics.h"
#include "../core/Event.h"

FileBrowser::FileBrowser(Application* app, const std::string& path)
    : Widget(Rect(0, 0, 600, 400)), m_app(app), m_selectedIndex(-1), m_scrollY(0) {
    if (path.empty() || path == "/") {
        // 根目录，显示驱动器
        m_currentPath = "::{20D04FE0-3AEA-1069-A2D8-08002B30309D}";
        m_files = FileSystem::getRootDrives();
    } else {
        navigateTo(path);
    }
}

FileBrowser::~FileBrowser() = default;

void FileBrowser::render() {
    if (!m_visible) return;
    
    // 白色背景
    Graphics::drawRect(m_rect, {255, 255, 255, 255});
    
    // 地址栏
    Rect addressBar(m_rect.x, m_rect.y, m_rect.w, 24);
    Graphics::drawRect(addressBar, XPColors::ButtonFace);
    Graphics::drawText("地址: " + m_currentPath, addressBar.x + 4, addressBar.y + 4, XPColors::TextBlack, 12);
    
    // 文件列表
    Rect listRect(m_rect.x, m_rect.y + 24, m_rect.w, m_rect.h - 24);
    Graphics::drawRect(listRect, {255, 255, 255, 255});
    
    i32 y = listRect.y - m_scrollY;
    for (i32 i = 0; i < m_files.size(); i++) {
        auto& file = m_files[i];
        Rect itemRect(listRect.x, y, listRect.w, ITEM_HEIGHT);
        
        if (i == m_selectedIndex) {
            Graphics::drawRect(itemRect, XPColors::SelectionBlue);
            Graphics::drawText(file.name, itemRect.x + 24, itemRect.y + 3, XPColors::SelectionText, 12);
        } else {
            // 图标
            if (file.type == FileType::Drive || file.type == FileType::Directory) {
                // 文件夹/驱动器图标
                Graphics::drawRect(Rect(itemRect.x + 4, itemRect.y + 4, 16, 16), {255, 220, 100, 255});
            } else {
                // 文件图标
                Graphics::drawRect(Rect(itemRect.x + 4, itemRect.y + 4, 16, 16), {255, 255, 255, 255});
                Graphics::drawRect(Rect(itemRect.x + 4, itemRect.y + 4, 16, 16), {200, 200, 200, 255}, false);
            }
            Graphics::drawText(file.name, itemRect.x + 24, itemRect.y + 3, XPColors::TextBlack, 12);
        }
        
        y += ITEM_HEIGHT;
    }
    
    Widget::render();
}

void FileBrowser::handleEvent(const Event& e) {
    Widget::handleEvent(e);
    
    Rect listRect(m_rect.x, m_rect.y + 24, m_rect.w, m_rect.h - 24);
    
    if (e.type == EventType::MouseClick && listRect.contains(e.position)) {
        i32 index = (e.position.y - listRect.y + m_scrollY) / ITEM_HEIGHT;
        if (index >= 0 && index < m_files.size()) {
            m_selectedIndex = index;
        }
    } else if (e.type == EventType::MouseDoubleClick && listRect.contains(e.position)) {
        i32 index = (e.position.y - listRect.y + m_scrollY) / ITEM_HEIGHT;
        if (index >= 0 && index < m_files.size()) {
            auto& file = m_files[index];
            if (file.type == FileType::Drive || file.type == FileType::Directory) {
                navigateTo(file.path);
            } else {
                // 打开文件
                std::string ext = file.path.substr(file.path.find_last_of('.'));
                if (ext == ".txt" || ext == ".log") {
                    m_app->openNotepad(file.path);
                } else if (ext == ".bmp" || ext == ".png" || ext == ".jpg") {
                    m_app->openImageViewer(file.path);
                }
            }
        }
    }
}

void FileBrowser::navigateTo(const std::string& path) {
    if (path == "::{20D04FE0-3AEA-1069-A2D8-08002B30309D}") {
        m_currentPath = "我的电脑";
        m_files = FileSystem::getRootDrives();
    } else {
        m_currentPath = path;
        m_files = FileSystem::listDirectory(path);
    }
    m_selectedIndex = -1;
    m_scrollY = 0;
}

void FileBrowser::refreshList() {
    if (m_currentPath == "我的电脑") {
        m_files = FileSystem::getRootDrives();
    } else {
        m_files = FileSystem::listDirectory(m_currentPath);
    }
}
