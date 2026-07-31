#pragma once

#include "../core/Window.h"
#include "../ui/Widget.h"
#include "../core/FileSystem.h"
#include <string>
#include <vector>

class FileBrowser : public Widget {
public:
    FileBrowser(Application* app, const std::string& path = "");
    ~FileBrowser() override;
    
    void render() override;
    void handleEvent(const Event& e) override;
    
private:
    void navigateTo(const std::string& path);
    void refreshList();
    
    Application* m_app;
    std::string m_currentPath;
    std::vector<FileInfo> m_files;
    i32 m_selectedIndex;
    i32 m_scrollY;
    static constexpr i32 ITEM_HEIGHT = 24;
};
