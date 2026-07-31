#pragma once

#include "../core/Window.h"
#include "../ui/Widget.h"
#include <string>
#include <fstream>

class Notepad : public Widget {
public:
    Notepad(Application* app, const std::string& filePath = "");
    ~Notepad() override;
    
    void render() override;
    void handleEvent(const Event& e) override;
    
private:
    void loadFile(const std::string& path);
    void saveFile();
    void newFile();
    
    Application* m_app;
    std::string m_currentFile;
    std::string m_text;
    i32 m_cursorPos;
    i32 m_scrollY;
    bool m_modified;
    static constexpr i32 LINE_HEIGHT = 18;
};
