#pragma once

#include "Types.h"
#include <queue>
#include <chrono>

struct Event {
    EventType type;
    Point position;
    MouseButton button;
    SDL_Keycode key;
    std::string text;
    u32 timestamp;
    
    Event() : type(EventType::None), button(MouseButton::Left), key(0), timestamp(0) {}
};

class EventSystem {
public:
    static void init();
    static void pollEvents();
    static bool hasEvent();
    static Event getEvent();
    
    // 触屏长按检测
    static void updateLongPress();
    static bool isLongPressTriggered();
    
private:
    static std::queue<Event> s_events;
    static bool s_mouseDown;
    static Point s_mouseDownPos;
    static std::chrono::steady_clock::time_point s_mouseDownTime;
    static bool s_longPressTriggered;
    static constexpr u32 LONG_PRESS_MS = 500;
};
