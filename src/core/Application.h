#pragma once

#include "Types.h"
#include "Window.h"
#include "../ui/Desktop.h"
#include "../ui/TaskBar.h"
#include <vector>
#include <memory>

class Application {
public:
    Application();
    ~Application();
    
    bool init();
    void run();
    void quit();
    
    // 窗口管理
    void addWindow(Window* window);
    void closeWindow(Window* window);
    void activateWindow(Window* window);
    void updateTaskBar();
    
    i32 getScreenWidth() const;
    i32 getScreenHeight() const;
    i32 getTaskBarHeight() const;
    
    // 打开内置应用
    void openNotepad(const std::string& filePath = "");
    void openCalculator();
    void openPaint();
    void openImageViewer(const std::string& filePath = "");
    void openBrowser();
    void openRunDialog();
    void openAbout();
    void openFileBrowser(const std::string& path = "");
    
public:
    SDL_Window* m_sdlWindow;
    SDL_Renderer* m_renderer;
    bool m_running;
    i32 m_screenWidth, m_screenHeight;
    
    std::unique_ptr<Desktop> m_desktop;
    std::unique_ptr<TaskBar> m_taskBar;
    std::vector<std::unique_ptr<Window>> m_windows;
    Window* m_activeWindow;
    
    u32 m_lastFrameTime;
};

extern Application* g_app;
