#include "Paint.h"
#include "../core/Graphics.h"
#include "../core/Event.h"

Paint::Paint(Application* app)
    : Widget(Rect(0, 0, 640, 480)), m_app(app), m_drawing(false), 
      m_currentColor{0, 0, 0, 255}, m_brushSize(2) {
    // 调色板颜色
    m_colors = {{
        {0, 0, 0, 255},       // 黑
        {128, 128, 128, 255}, // 灰
        {128, 0, 0, 255},     // 暗红
        {255, 0, 0, 255},     // 红
        {0, 128, 0, 255},     // 暗绿
        {0, 255, 0, 255},     // 绿
        {0, 0, 128, 255},     // 暗蓝
        {0, 0, 255, 255}      // 蓝
    }};
    
    m_canvasRect = Rect(0, 40, m_rect.w, m_rect.h - 40);
}

Paint::~Paint() = default;

void Paint::render() {
    if (!m_visible) return;
    
    // 工具栏背景
    Graphics::drawRect(Rect(m_rect.x, m_rect.y, m_rect.w, 40), XPColors::ButtonFace);
    
    // 调色板
    i32 colorX = m_rect.x + 10;
    i32 colorY = m_rect.y + 8;
    for (i32 i = 0; i < m_colors.size(); i++) {
        Rect colorRect(colorX + i * 24, colorY, 20, 24);
        Graphics::drawRect(colorRect, m_colors[i]);
        Graphics::drawRect(colorRect, XPColors::ButtonShadow, false);
        
        if (m_colors[i].r == m_currentColor.r && 
            m_colors[i].g == m_currentColor.g && 
            m_colors[i].b == m_currentColor.b) {
            // 选中的颜色加边框
            Graphics::drawRect(Rect(colorRect.x - 2, colorRect.y - 2, colorRect.w + 4, colorRect.h + 4),
                              XPColors::SelectionBlue, false);
        }
    }
    
    // 画布白色背景
    Graphics::drawRect(m_canvasRect, {255, 255, 255, 255});
    
    // 绘制所有点
    for (auto& p : m_points) {
        for (i32 dx = -m_brushSize; dx <= m_brushSize; dx++) {
            for (i32 dy = -m_brushSize; dy <= m_brushSize; dy++) {
                if (dx*dx + dy*dy <= m_brushSize*m_brushSize) {
                    SDL_SetRenderDrawColor(Graphics::s_renderer, p.color.r, p.color.g, p.color.b, p.color.a);
                    SDL_RenderDrawPoint(Graphics::s_renderer, m_canvasRect.x + p.x + dx, m_canvasRect.y + p.y + dy);
                }
            }
        }
    }
    
    Widget::render();
}

void Paint::handleEvent(const Event& e) {
    if (!m_visible) return;
    
    Widget::handleEvent(e);
    
    // 转换坐标到画布
    Point canvasPos(e.position.x - m_canvasRect.x, e.position.y - m_canvasRect.y);
    
    // 调色板点击
    i32 colorX = m_rect.x + 10;
    i32 colorY = m_rect.y + 8;
    for (i32 i = 0; i < m_colors.size(); i++) {
        Rect colorRect(colorX + i * 24, colorY, 20, 24);
        if (e.type == EventType::MouseClick && colorRect.contains(e.position)) {
            m_currentColor = m_colors[i];
            return;
        }
    }
    
    // 画布绘图
    if (m_canvasRect.contains(e.position)) {
        if (e.type == EventType::MouseDown && e.button == MouseButton::Left) {
            m_drawing = true;
            m_points.push_back({canvasPos.x, canvasPos.y, m_currentColor, m_brushSize});
        } else if (e.type == EventType::MouseUp) {
            m_drawing = false;
        } else if (e.type == EventType::MouseMove && m_drawing) {
            m_points.push_back({canvasPos.x, canvasPos.y, m_currentColor, m_brushSize});
        }
    }
}
