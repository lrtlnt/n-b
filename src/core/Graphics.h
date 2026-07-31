#pragma once

#include "Types.h"
#include <SDL2/SDL_ttf.h>
#include <string>

class Graphics {
public:
    static void init(SDL_Renderer* renderer);
    static void shutdown();
    
    // 绘制函数
    static void drawRect(const Rect& rect, SDL_Color color, bool filled = true);
    static void drawGradientRect(const Rect& rect, SDL_Color startColor, SDL_Color endColor, bool vertical = true);
    static void draw3DButton(const Rect& rect, bool pressed = false, bool hovered = false);
    static void drawLine(i32 x1, i32 y1, i32 x2, i32 y2, SDL_Color color);
    static void drawText(const std::string& text, i32 x, i32 y, SDL_Color color, i32 fontSize = 12, bool centered = false);
    static void drawTextInRect(const std::string& text, const Rect& rect, SDL_Color color, i32 fontSize = 12);
    
    // 资源管理
    static TTF_Font* getFont(i32 size);
    static SDL_Texture* loadTexture(const std::string& path);
    static SDL_Texture* createTextureFromSurface(SDL_Surface* surface);
    
    // 绘制Bliss壁纸效果
    static void drawBlissWallpaper(i32 width, i32 height);
    
public:
    static SDL_Renderer* s_renderer;
private:
    static std::unordered_map<i32, TTF_Font*> s_fonts;
};
