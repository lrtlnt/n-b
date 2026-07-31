#include "Application.h"
#include "Graphics.h"
#include "Event.h"
#include "FileSystem.h"
#include "../apps/Notepad.h"
#include "../apps/Calculator.h"
#include "../apps/Paint.h"
#include "../apps/ImageViewer.h"
#include "../apps/About.h"
#include "../apps/RunDialog.h"
#include "../apps/FileBrowser.h"
#include <SDL2/SDL.h>
#include <iostream>

Application* g_app = nullptr;

Application::Application()
    : m_sdlWindow(nullptr), m_renderer(nullptr), m_running(false),
      m_activeWindow(nullptr), m_lastFrameTime(0) {
    g_app = this;
}

Application::~Application() {
    Graphics::shutdown();
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_sdlWindow) {
        SDL_DestroyWindow(m_sdlWindow);
    }
    SDL_Quit();
}

bool Application::init() {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // 获取屏幕分辨率
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    m_screenWidth = dm.w;
    m_screenHeight = dm.h;
    
    // 创建窗口，全屏（Android自动全屏，Windows可以窗口化？不对，用户要求Android自动全屏，Windows可以默认窗口化？不，模拟器本身是一个窗口，里面是XP桌面，所以应该创建一个窗口，大小可以是默认1024x768，或者全屏？不对，应该是SDL窗口作为模拟器的容器，里面绘制整个XP桌面，所以Windows下可以是窗口，Android下全屏。
#ifdef __ANDROID__
    m_sdlWindow = SDL_CreateWindow("Windows XP Simulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI);
#else
    m_sdlWindow = SDL_CreateWindow("Windows XP Simulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1024, 768, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_screenWidth = 1024;
    m_screenHeight = 768;
#endif
    
    if (!m_sdlWindow) {
        std::cerr << "Window create failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // 创建渲染器
    m_renderer = SDL_CreateRenderer(m_sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "Renderer create failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    
    // 初始化图形系统
    Graphics::init(m_renderer);
    EventSystem::init();
    FileSystem::init();
    
    // 创建桌面和任务栏
    m_desktop = std::make_unique<Desktop>(m_screenWidth, m_screenHeight, 40);
    m_taskBar = std::make_unique<TaskBar>(m_screenWidth, m_screenHeight);
    
    m_running = true;
    m_lastFrameTime = SDL_GetTicks();
    
    return true;
}

void Application::run() {
    while (m_running) {
        u32 currentTime = SDL_GetTicks();
        u32 deltaTime = currentTime - m_lastFrameTime;
        m_lastFrameTime = currentTime;
        
        // 处理事件
        EventSystem::pollEvents();
        while (EventSystem::hasEvent()) {
            Event e = EventSystem::getEvent();
            
            if (e.type == EventType::Quit) {
                m_running = false;
                break;
            }
            
            // 先给最上层窗口处理事件
            bool handled = false;
            for (auto it = m_windows.rbegin(); it != m_windows.rend(); ++it) {
                if ((*it)->isVisible() && !(*it)->isMinimized()) {
                    (*it)->handleEvent(e);
                    handled = true;
                    break;
                }
            }
            
            // 如果窗口没处理，给任务栏和桌面处理
            if (!handled) {
                m_taskBar->handleEvent(e);
                m_desktop->handleEvent(e);
            }
        }
        
        // 更新
        for (auto& window : m_windows) {
            window->update(deltaTime);
        }
        m_desktop->update(deltaTime);
        m_taskBar->update(deltaTime);
        
        // 渲染
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);
        
        // 渲染桌面
        m_desktop->render();
        
        // 渲染窗口（按z序）
        for (auto& window : m_windows) {
            window->render();
        }
        
        // 渲染任务栏
        m_taskBar->render();
        
        SDL_RenderPresent(m_renderer);
        
        SDL_Delay(1);
    }
}

void Application::quit() {
    m_running = false;
}

void Application::addWindow(Window* window) {
    m_windows.emplace_back(window);
    m_taskBar->addWindow(window);
    activateWindow(window);
}

void Application::closeWindow(Window* window) {
    m_taskBar->removeWindow(window);
    for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
        if (it->get() == window) {
            m_windows.erase(it);
            break;
        }
    }
    
    // 激活下一个窗口
    if (!m_windows.empty()) {
        activateWindow(m_windows.back().get());
    } else {
        m_activeWindow = nullptr;
    }
}

void Application::activateWindow(Window* window) {
    // 把窗口移到最上层
    for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
        if (it->get() == window) {
            auto win = std::move(*it);
            m_windows.erase(it);
            m_windows.push_back(std::move(win));
            break;
        }
    }
    
    // 取消其他窗口的激活状态
    for (auto& win : m_windows) {
        win->deactivate();
    }
    window->m_isActive = true;
    m_activeWindow = window;
    updateTaskBar();
}

void Application::updateTaskBar() {
    for (auto& window : m_windows) {
        m_taskBar->updateWindowButton(window.get());
    }
}

i32 Application::getScreenWidth() const {
    return m_screenWidth;
}

i32 Application::getScreenHeight() const {
    return m_screenHeight;
}

i32 Application::getTaskBarHeight() const {
    return m_taskBar->getHeight();
}

// 内置应用实现
void Application::openNotepad(const std::string& filePath) {
    auto* notepad = new Notepad(this, filePath);
    addWindow(notepad);
}

void Application::openCalculator() {
    auto* calc = new Calculator(this);
    addWindow(calc);
}

void Application::openPaint() {
    auto* paint = new Paint(this);
    addWindow(paint);
}

void Application::openImageViewer(const std::string& filePath) {
    auto* viewer = new ImageViewer(this, filePath);
    addWindow(viewer);
}

void Application::openBrowser() {
    // 简单浏览器暂时用提示窗口代替，libcurl后面可以加，先简单实现
    auto* about = new About(this, "浏览器", "简单浏览器功能开发中...\n后续版本将支持网页浏览。");
    addWindow(about);
}

void Application::openRunDialog() {
    auto* run = new RunDialog(this);
    addWindow(run);
}

void Application::openAbout() {
    auto* about = new About(this, "关于 Windows XP",
        "Microsoft Windows XP\n版本 2002 Service Pack 3\n\n"
        "本模拟器由 C++ + SDL2 开发\n"
        "支持 Windows 和 Android 跨平台运行\n"
        "© 2026 XP Simulator Project");
    addWindow(about);
}

void Application::openFileBrowser(const std::string& path) {
    auto* browser = new FileBrowser(this, path);
    addWindow(browser);
}
