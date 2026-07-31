#include "ImageViewer.h"
#include "../core/Graphics.h"
#include "../core/Event.h"

ImageViewer::ImageViewer(Application* app, const std::string& filePath)
    : Widget(Rect(0, 0, 600, 450)), m_app(app), m_texture(nullptr), 
      m_imgWidth(0), m_imgHeight(0), m_filePath(filePath) {
    if (!filePath.empty()) {
        SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (surface) {
            m_texture = Graphics::createTextureFromSurface(surface);
            m_imgWidth = surface->w;
            m_imgHeight = surface->h;
            SDL_FreeSurface(surface);
        }
    }
}

ImageViewer::~ImageViewer() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

void ImageViewer::render() {
    if (!m_visible) return;
    
    // 灰色背景
    Graphics::drawRect(m_rect, {200, 200, 200, 255});
    
    if (m_texture) {
        // 居中显示，适应窗口
        f32 scaleX = (f32)m_rect.w / m_imgWidth;
        f32 scaleY = (f32)m_rect.h / m_imgHeight;
        f32 scale = min(scaleX, scaleY);
        
        i32 drawW = m_imgWidth * scale;
        i32 drawH = m_imgHeight * scale;
        i32 drawX = m_rect.x + (m_rect.w - drawW) / 2;
        i32 drawY = m_rect.y + (m_rect.h - drawH) / 2;
        
        SDL_Rect dstRect = {drawX, drawY, drawW, drawH};
        SDL_RenderCopy(Graphics::s_renderer, m_texture, nullptr, &dstRect);
    } else {
        Graphics::drawText("无法加载图片", m_rect.x + m_rect.w/2, m_rect.y + m_rect.h/2,
                          XPColors::TextBlack, 16, true);
    }
    
    Widget::render();
}

void ImageViewer::handleEvent(const Event& e) {
    Widget::handleEvent(e);
}
