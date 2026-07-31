#pragma once

#include "../core/Window.h"
#include "../ui/Widget.h"
#include <vector>
#include <array>

struct DrawPoint {
    i32 x, y;
    SDL_Color color;
    i32 size;
};

class Paint : public Widget {
public:
    Paint(Application* app);
    ~Paint() override;
    
    void render() override;
    void handleEvent(const Event& e) override;
    
private:
    Application* m_app;
    std::vector<DrawPoint> m_points;
    bool m_drawing;
    SDL_Color m_currentColor;
    i32 m_brushSize;
    std::array<SDL_Color, 8> m_colors;
    Rect m_canvasRect;
};
