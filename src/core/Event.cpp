#include "Event.h"
#include <SDL2/SDL.h>

std::queue<Event> EventSystem::s_events;
bool EventSystem::s_mouseDown = false;
Point EventSystem::s_mouseDownPos;
std::chrono::steady_clock::time_point EventSystem::s_mouseDownTime;
bool EventSystem::s_longPressTriggered = false;

void EventSystem::init() {
    s_mouseDown = false;
    s_longPressTriggered = false;
}

void EventSystem::pollEvents() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        Event e;
        e.timestamp = SDL_GetTicks();
        
        switch (sdlEvent.type) {
            case SDL_QUIT:
                e.type = EventType::Quit;
                s_events.push(e);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                    e.type = EventType::MouseDown;
                    e.button = MouseButton::Left;
                    e.position = Point(sdlEvent.button.x, sdlEvent.button.y);
                    s_events.push(e);
                    
                    s_mouseDown = true;
                    s_mouseDownPos = e.position;
                    s_mouseDownTime = std::chrono::steady_clock::now();
                    s_longPressTriggered = false;
                } else if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
                    e.type = EventType::RightClick;
                    e.button = MouseButton::Right;
                    e.position = Point(sdlEvent.button.x, sdlEvent.button.y);
                    s_events.push(e);
                }
                break;
                
            case SDL_MOUSEBUTTONUP:
                if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                    if (s_longPressTriggered) {
                        // 长按已经触发右键，忽略这次左键up
                        s_mouseDown = false;
                        s_longPressTriggered = false;
                        break;
                    }
                    
                    e.type = EventType::MouseUp;
                    e.button = MouseButton::Left;
                    e.position = Point(sdlEvent.button.x, sdlEvent.button.y);
                    s_events.push(e);
                    
                    // 判断点击和双击
                    static u32 lastClickTime = 0;
                    static Point lastClickPos;
                    u32 now = SDL_GetTicks();
                    
                    if (now - lastClickTime < 500 && 
                        abs(e.position.x - lastClickPos.x) < 5 && 
                        abs(e.position.y - lastClickPos.y) < 5) {
                        e.type = EventType::MouseDoubleClick;
                        s_events.push(e);
                        lastClickTime = 0;
                    } else {
                        e.type = EventType::MouseClick;
                        s_events.push(e);
                        lastClickTime = now;
                        lastClickPos = e.position;
                    }
                    
                    s_mouseDown = false;
                }
                break;
                
            case SDL_MOUSEMOTION:
                e.type = EventType::MouseMove;
                e.position = Point(sdlEvent.motion.x, sdlEvent.motion.y);
                s_events.push(e);
                
                // 如果移动超过阈值，取消长按
                if (s_mouseDown) {
                    if (abs(e.position.x - s_mouseDownPos.x) > 10 || 
                        abs(e.position.y - s_mouseDownPos.y) > 10) {
                        s_mouseDown = false;
                    }
                }
                break;
                
            case SDL_KEYDOWN:
                e.type = EventType::KeyDown;
                e.key = sdlEvent.key.keysym.sym;
                s_events.push(e);
                break;
                
            case SDL_KEYUP:
                e.type = EventType::KeyUp;
                e.key = sdlEvent.key.keysym.sym;
                s_events.push(e);
                break;
                
            case SDL_TEXTINPUT:
                e.type = EventType::TextInput;
                e.text = sdlEvent.text.text;
                s_events.push(e);
                break;
                
            case SDL_WINDOWEVENT:
                e.type = EventType::WindowEvent;
                s_events.push(e);
                break;
        }
    }
    
    updateLongPress();
}

void EventSystem::updateLongPress() {
    if (s_mouseDown && !s_longPressTriggered) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_mouseDownTime).count();
        
        if (duration >= LONG_PRESS_MS) {
            // 触发右键事件
            Event e;
            e.type = EventType::RightClick;
            e.button = MouseButton::Right;
            e.position = s_mouseDownPos;
            e.timestamp = SDL_GetTicks();
            s_events.push(e);
            
            s_longPressTriggered = true;
            s_mouseDown = false;
        }
    }
}

bool EventSystem::isLongPressTriggered() {
    return s_longPressTriggered;
}

bool EventSystem::hasEvent() {
    return !s_events.empty();
}

Event EventSystem::getEvent() {
    if (s_events.empty()) {
        return Event();
    }
    Event e = s_events.front();
    s_events.pop();
    return e;
}
