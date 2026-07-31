#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>

// 基础类型定义
using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

// 点和矩形结构
struct Point {
    i32 x, y;
    Point() : x(0), y(0) {}
    Point(i32 x, i32 y) : x(x), y(y) {}
};

struct Rect {
    i32 x, y, w, h;
    Rect() : x(0), y(0), w(0), h(0) {}
    Rect(i32 x, i32 y, i32 w, i32 h) : x(x), y(y), w(w), h(h) {}
    
    bool contains(const Point& p) const {
        return p.x >= x && p.x < x + w && p.y >= y && p.y < y + h;
    }
    
    bool intersects(const Rect& r) const {
        return x < r.x + r.w && x + w > r.x && y < r.y + r.h && y + h > r.y;
    }
};

// XP主题颜色常量
namespace XPColors {
    // 标题栏渐变
    constexpr SDL_Color TitleBarActiveStart = {10, 36, 106, 255};   // #0A246A
    constexpr SDL_Color TitleBarActiveEnd = {58, 110, 165, 255};   // #3A6EA5
    constexpr SDL_Color TitleBarInactiveStart = {128, 128, 128, 255};
    constexpr SDL_Color TitleBarInactiveEnd = {192, 192, 192, 255};
    
    // 选中项颜色
    constexpr SDL_Color SelectionBlue = {49, 106, 197, 255};       // #316AC5
    constexpr SDL_Color SelectionText = {255, 255, 255, 255};
    
    // 按钮颜色
    constexpr SDL_Color ButtonFace = {236, 233, 216, 255};
    constexpr SDL_Color ButtonHighlight = {255, 255, 255, 255};
    constexpr SDL_Color ButtonShadow = {128, 128, 128, 255};
    constexpr SDL_Color ButtonDarkShadow = {64, 64, 64, 255};
    
    // 任务栏颜色
    constexpr SDL_Color TaskBarStart = {35, 105, 210, 255};
    constexpr SDL_Color TaskBarEnd = {10, 60, 150, 255};
    constexpr SDL_Color StartButtonGreen = {30, 160, 30, 255};
    constexpr SDL_Color StartButtonGreenHover = {40, 190, 40, 255};
    
    // 关闭按钮悬停红色
    constexpr SDL_Color CloseButtonHover = {220, 50, 50, 255};
    
    // 菜单颜色
    constexpr SDL_Color MenuBackground = {255, 255, 255, 255};
    constexpr SDL_Color MenuBorder = {128, 128, 128, 255};
    constexpr SDL_Color MenuSeparator = {200, 200, 200, 255};
    
    // 桌面颜色
    constexpr SDL_Color DesktopBackground = {0, 70, 120, 255}; // Bliss蓝天基础色
    
    // 窗口颜色
    constexpr SDL_Color WindowBackground = {255, 255, 255, 255};
    constexpr SDL_Color WindowBorder = {0, 0, 0, 255};
    
    // 文本颜色
    constexpr SDL_Color TextBlack = {0, 0, 0, 255};
    constexpr SDL_Color TextWhite = {255, 255, 255, 255};
    constexpr SDL_Color TextGray = {128, 128, 128, 255};
}

// 事件类型
enum class EventType {
    None,
    MouseDown,
    MouseUp,
    MouseMove,
    MouseClick,
    MouseDoubleClick,
    RightClick,
    KeyDown,
    KeyUp,
    TextInput,
    Quit,
    WindowEvent,
    Timer
};

// 鼠标按键
enum class MouseButton {
    Left,
    Middle,
    Right
};

// 窗口状态
enum class WindowState {
    Normal,
    Minimized,
    Maximized
};

// 文件类型
enum class FileType {
    File,
    Directory,
    Drive
};

// 文件信息结构
struct FileInfo {
    std::string name;
    std::string path;
    FileType type;
    u64 size;
    u64 modifiedTime;
};

// 前向声明
class Window;
class Widget;
class Application;
