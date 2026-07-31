#pragma once

#include "../core/Window.h"
#include "../ui/Widget.h"
#include <SDL2/SDL_image.h>
#include <string>

class ImageViewer : public Widget {
public:
    ImageViewer(Application* app, const std::string& filePath = "");
    ~ImageViewer() override;
    
    void render() override;
    void handleEvent(const Event& e) override;
    
private:
    Application* m_app;
    SDL_Texture* m_texture;
    i32 m_imgWidth, m_imgHeight;
    std::string m_filePath;
};
