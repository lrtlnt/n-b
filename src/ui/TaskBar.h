#pragma once

#include "Widget.h"
#include "Button.h"
#include "ContextMenu.h"
#include <vector>
#include <memory>

class Window;

class TaskBar : public Widget {
public:
    TaskBar(i32 screenWidth, i32 screenHeight);
    ~TaskBar();
    
    void render() override;
    void handleEvent(const Event& e) override;
    void update(u32 deltaTime) override;
    
    // 窗口管理
    void addWindow(Window* window);
    void removeWindow(Window* window);
    void updateWindowButton(Window* window);
    
    i32 getHeight() const;
    
    // 显示开始菜单
    void showStartMenu();
    
private:
    i32 m_height;
    std::unique_ptr<Button> m_startButton;
    std::unique_ptr<ContextMenu> m_startMenu;
    std::vector<std::pair<Window*, std::unique_ptr<Button>>> m_windowButtons;
    std::string m_currentTime;
    u32 m_timeUpdateTimer;
    static constexpr i32 TASKBAR_HEIGHT = 40;
};
