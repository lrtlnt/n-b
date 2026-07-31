#include "Graphics.h"
#include <cmath>
#include <unordered_map>

SDL_Renderer* Graphics::s_renderer = nullptr;
std::unordered_map<i32, TTF_Font*> Graphics::s_fonts;

void Graphics::init(SDL_Renderer* renderer) {
    s_renderer = renderer;
    TTF_Init();
    // 预加载常用字体
    getFont(12);
    getFont(14);
    getFont(16);
}

void Graphics::shutdown() {
    for (auto& pair : s_fonts) {
        TTF_CloseFont(pair.second);
    }
    s_fonts.clear();
    TTF_Quit();
}

void Graphics::drawRect(const Rect& rect, SDL_Color color, bool filled) {
    SDL_SetRenderDrawColor(s_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect sdlRect = {rect.x, rect.y, rect.w, rect.h};
    if (filled) {
        SDL_RenderFillRect(s_renderer, &sdlRect);
    } else {
        SDL_RenderDrawRect(s_renderer, &sdlRect);
    }
}

void Graphics::drawGradientRect(const Rect& rect, SDL_Color startColor, SDL_Color endColor, bool vertical) {
    if (vertical) {
        for (i32 y = rect.y; y < rect.y + rect.h; y++) {
            f32 t = (f32)(y - rect.y) / rect.h;
            SDL_Color c;
            c.r = startColor.r + (endColor.r - startColor.r) * t;
            c.g = startColor.g + (endColor.g - startColor.g) * t;
            c.b = startColor.b + (endColor.b - startColor.b) * t;
            c.a = 255;
            SDL_SetRenderDrawColor(s_renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawLine(s_renderer, rect.x, y, rect.x + rect.w, y);
        }
    } else {
        for (i32 x = rect.x; x < rect.x + rect.w; x++) {
            f32 t = (f32)(x - rect.x) / rect.w;
            SDL_Color c;
            c.r = startColor.r + (endColor.r - startColor.r) * t;
            c.g = startColor.g + (endColor.g - startColor.g) * t;
            c.b = startColor.b + (endColor.b - startColor.b) * t;
            c.a = 255;
            SDL_SetRenderDrawColor(s_renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawLine(s_renderer, x, rect.y, x, rect.y + rect.h);
        }
    }
}

void Graphics::draw3DButton(const Rect& rect, bool pressed, bool hovered) {
    // 按钮背景
    SDL_Color faceColor = XPColors::ButtonFace;
    if (hovered) {
        faceColor.r = min(255, faceColor.r + 20);
        faceColor.g = min(255, faceColor.g + 20);
        faceColor.b = min(255, faceColor.b + 20);
    }
    drawRect(rect, faceColor);
    
    if (pressed) {
        // 按下状态：深色边框在左上，浅色在右下
        drawLine(rect.x, rect.y, rect.x + rect.w - 1, rect.y, XPColors::ButtonDarkShadow);
        drawLine(rect.x, rect.y, rect.x, rect.y + rect.h - 1, XPColors::ButtonDarkShadow);
        drawLine(rect.x + 1, rect.y + 1, rect.x + rect.w - 2, rect.y + 1, XPColors::ButtonShadow);
        drawLine(rect.x + 1, rect.y + 1, rect.x + 1, rect.y + rect.h - 2, XPColors::ButtonShadow);
    } else {
        // 正常状态：浅色边框在左上，深色在右下
        drawLine(rect.x, rect.y, rect.x + rect.w - 1, rect.y, XPColors::ButtonHighlight);
        drawLine(rect.x, rect.y, rect.x, rect.y + rect.h - 1, XPColors::ButtonHighlight);
        drawLine(rect.x + 1, rect.y + 1, rect.x + rect.w - 2, rect.y + 1, XPColors::ButtonHighlight);
        drawLine(rect.x + 1, rect.y + 1, rect.x + 1, rect.y + rect.h - 2, XPColors::ButtonHighlight);
        
        drawLine(rect.x, rect.y + rect.h - 1, rect.x + rect.w - 1, rect.y + rect.h - 1, XPColors::ButtonDarkShadow);
        drawLine(rect.x + rect.w - 1, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1, XPColors::ButtonDarkShadow);
        drawLine(rect.x + 1, rect.y + rect.h - 2, rect.x + rect.w - 2, rect.y + rect.h - 2, XPColors::ButtonShadow);
        drawLine(rect.x + rect.w - 2, rect.y + 1, rect.x + rect.w - 2, rect.y + rect.h - 2, XPColors::ButtonShadow);
    }
}

void Graphics::drawLine(i32 x1, i32 y1, i32 x2, i32 y2, SDL_Color color) {
    SDL_SetRenderDrawColor(s_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(s_renderer, x1, y1, x2, y2);
}

TTF_Font* Graphics::getFont(i32 size) {
    if (s_fonts.count(size)) {
        return s_fonts[size];
    }
    // 使用系统默认字体，Windows下用Tahoma，Android下用Roboto，其他用sans
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/tahoma.ttf", size);
    if (!font) {
        font = TTF_OpenFont("/system/fonts/Roboto-Regular.ttf", size);
    }
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", size);
    }
    if (!font) {
        // 最后尝试内置默认字体
        font = TTF_OpenFont("fonts/Default.ttf", size);
    }
    if (font) {
        s_fonts[size] = font;
    }
    return font;
}

void Graphics::drawText(const std::string& text, i32 x, i32 y, SDL_Color color, i32 fontSize, bool centered) {
    TTF_Font* font = getFont(fontSize);
    if (!font) return;
    
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(s_renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    
    i32 drawX = x;
    i32 drawY = y;
    if (centered) {
        drawX -= surface->w / 2;
        drawY -= surface->h / 2;
    }
    
    SDL_Rect dstRect = {drawX, drawY, surface->w, surface->h};
    SDL_RenderCopy(s_renderer, texture, nullptr, &dstRect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Graphics::drawTextInRect(const std::string& text, const Rect& rect, SDL_Color color, i32 fontSize) {
    TTF_Font* font = getFont(fontSize);
    if (!font) return;
    
    i32 textWidth = 0;
    TTF_SizeText(font, text.c_str(), &textWidth, nullptr);
    
    i32 x = rect.x + 4;
    i32 y = rect.y + (rect.h - fontSize) / 2;
    
    // 如果文本太长，截断
    std::string drawText = text;
    if (textWidth > rect.w - 8) {
        while (drawText.size() > 3 && textWidth > rect.w - 20) {
            drawText = drawText.substr(0, drawText.size() - 1);
            TTF_SizeText(font, (drawText + "...").c_str(), &textWidth, nullptr);
        }
        drawText += "...";
    }
    
    drawText(drawText, x, y, color, fontSize);
}

SDL_Texture* Graphics::loadTexture(const std::string& path) {
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(s_renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* Graphics::createTextureFromSurface(SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(s_renderer, surface);
}

void Graphics::drawBlissWallpaper(i32 width, i32 height) {
    // 绘制蓝天渐变
    drawGradientRect(Rect(0, 0, width, height * 2/3), 
                     {0, 120, 200, 255}, {100, 180, 230, 255}, true);
    
    // 绘制白云（简单的圆形叠加）
    srand(12345); // 固定种子保证云位置不变
    for (int i = 0; i < 8; i++) {
        i32 cloudX = rand() % width;
        i32 cloudY = rand() % (height / 3);
        i32 cloudSize = 40 + rand() % 60;
        
        for (int dx = -cloudSize; dx <= cloudSize; dx += 10) {
            for (int dy = -cloudSize/2; dy <= cloudSize/2; dy += 10) {
                if (dx*dx + dy*dy*4 < cloudSize*cloudSize) {
                    SDL_SetRenderDrawColor(s_renderer, 255, 255, 255, 200);
                    SDL_RenderDrawPoint(s_renderer, cloudX + dx, cloudY + dy);
                }
            }
        }
    }
    
    // 绘制绿色草地渐变
    drawGradientRect(Rect(0, height * 2/3, width, height / 3),
                     {80, 160, 60, 255}, {40, 120, 40, 255}, true);
    
    // 绘制草地纹理
    for (int i = 0; i < 200; i++) {
        i32 grassX = rand() % width;
        i32 grassY = height * 2/3 + rand() % (height / 3);
        i32 grassHeight = 5 + rand() % 15;
        SDL_SetRenderDrawColor(s_renderer, 30 + rand()%40, 100 + rand()%60, 30 + rand()%40, 255);
        SDL_RenderDrawLine(s_renderer, grassX, grassY, grassX + (rand()%5 - 2), grassY - grassHeight);
    }
}
