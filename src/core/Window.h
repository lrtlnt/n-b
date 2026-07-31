#pragma once

#include "Types.h"
#include "../ui/Widget.h"
#include "../ui/Button.h"
#include <string>
#include <memory>

class Application;

class Window : public Widget {
public:
    Window(Application* app, const std::string& title, const Rect& rect);
    ~Window() override;
    
    void render() override;
    void handleEvent(const Event& e) override;
    void update(u32 deltaTime) override;
    
    // 窗口操作
    void setTitle(const std::string& title);
    std::string getTitle() const;
    
    void activate();
    void deactivate();
    bool isActive() const;
    
    void minimize();
    void maximize();
    void restore();
    void close();
    
    bool isMinimized() const;
    bool isMaximized() const;
    WindowState getState() const;
    
    Rect getClientRect() const;
    
    // 设置客户区内容
    void setContent(Widget* content);
    
protected:
    Application* m_app;
    std::string m_title;
    WindowState m_state;
    Rect m_restoreRect; // 最大化/最小化前的位置
    bool m_dragging;
    bool m_isActive;
    Point m_dragOffset;
    
    std::unique_ptr<Button> m_minButton;
    std::unique_ptr<Button> m_maxButton;
    std::unique_ptr<Button> m_closeButton;
    Widget* m_content;
    
    static constexpr i32 TITLEBAR_HEIGHT = 30;
    static constexpr i32 BORDER_WIDTH = 4;
};
