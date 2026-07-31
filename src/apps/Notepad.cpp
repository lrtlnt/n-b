#include "Notepad.h"
#include "../core/Graphics.h"
#include "../core/Event.h"
#include "../core/FileSystem.h"
#include <sstream>

Notepad::Notepad(Application* app, const std::string& filePath)
    : Widget(Rect(0, 0, 600, 400)), m_app(app), m_cursorPos(0), m_scrollY(0), m_modified(false) {
    if (!filePath.empty()) {
        loadFile(filePath);
    }
}

Notepad::~Notepad() = default;

void Notepad::render() {
    if (!m_visible) return;
    
    // 白色背景
    Graphics::drawRect(m_rect, {255, 255, 255, 255});
    
    // 绘制文本，按行分割
    std::istringstream stream(m_text);
    std::string line;
    i32 y = m_rect.y + 4 - m_scrollY;
    i32 lineNum = 0;
    
    while (std::getline(stream, line)) {
        if (y >= m_rect.y && y < m_rect.y + m_rect.h) {
            Graphics::drawText(line, m_rect.x + 4, y, XPColors::TextBlack, 14);
        }
        y += LINE_HEIGHT;
        lineNum++;
    }
    
    // 绘制光标
    // 简单计算光标位置
    i32 cursorX = m_rect.x + 4;
    i32 cursorY = m_rect.y + 4 - m_scrollY;
    std::string beforeCursor = m_text.substr(0, m_cursorPos);
    std::istringstream cursorStream(beforeCursor);
    std::string cursorLine;
    while (std::getline(cursorStream, cursorLine)) {
        TTF_Font* font = Graphics::getFont(14);
        i32 w = 0;
        TTF_SizeText(font, cursorLine.c_str(), &w, nullptr);
        cursorX = m_rect.x + 4 + w;
        cursorY += LINE_HEIGHT;
    }
    cursorY -= LINE_HEIGHT;
    
    if (SDL_GetTicks() % 1000 < 500) {
        Graphics::drawLine(cursorX, cursorY, cursorX, cursorY + LINE_HEIGHT - 2, XPColors::TextBlack);
    }
    
    Widget::render();
}

void Notepad::handleEvent(const Event& e) {
    if (!m_visible) return;
    
    Widget::handleEvent(e);
    
    switch (e.type) {
        case EventType::TextInput:
            m_text.insert(m_cursorPos, e.text);
            m_cursorPos += e.text.size();
            m_modified = true;
            break;
            
        case EventType::KeyDown:
            if (e.key == SDLK_BACKSPACE && m_cursorPos > 0) {
                m_text.erase(m_cursorPos - 1, 1);
                m_cursorPos--;
                m_modified = true;
            } else if (e.key == SDLK_DELETE && m_cursorPos < m_text.size()) {
                m_text.erase(m_cursorPos, 1);
                m_modified = true;
            } else if (e.key == SDLK_LEFT && m_cursorPos > 0) {
                m_cursorPos--;
            } else if (e.key == SDLK_RIGHT && m_cursorPos < m_text.size()) {
                m_cursorPos++;
            } else if (e.key == SDLK_RETURN) {
                m_text.insert(m_cursorPos, "\n");
                m_cursorPos++;
                m_modified = true;
            } else if (e.key == SDLK_s && (SDL_GetModState() & KMOD_CTRL)) {
                saveFile();
            } else if (e.key == SDLK_n && (SDL_GetModState() & KMOD_CTRL)) {
                newFile();
            }
            break;
            
        case EventType::MouseClick:
            if (m_rect.contains(e.position)) {
                // 简单设置光标位置，点击哪行就到哪行
                i32 clickY = e.position.y - m_rect.y - 4 + m_scrollY;
                i32 line = clickY / LINE_HEIGHT;
                m_cursorPos = 0;
                std::istringstream stream(m_text);
                std::string l;
                for (i32 i = 0; i <= line && std::getline(stream, l); i++) {
                    if (i < line) {
                        m_cursorPos += l.size() + 1; // +1 for newline
                    }
                }
                if (m_cursorPos > m_text.size()) m_cursorPos = m_text.size();
            }
            break;
            
        case EventType::MouseWheel:
            // 滚轮滚动
            break;
            
        default:
            break;
    }
}

void Notepad::loadFile(const std::string& path) {
    std::ifstream file(path);
    if (file.is_open()) {
        m_text.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        m_currentFile = path;
        m_cursorPos = m_text.size();
        m_modified = false;
        // 获取父窗口设置标题
        // 简单处理，后面可以通过Window指针设置
        file.close();
    }
}

void Notepad::saveFile() {
    if (m_currentFile.empty()) {
        // 另存为，简单保存到桌面
        m_currentFile = FileSystem::getDesktopPath() + "/未命名.txt";
    }
    std::ofstream file(m_currentFile);
    if (file.is_open()) {
        file << m_text;
        file.close();
        m_modified = false;
    }
}

void Notepad::newFile() {
    m_text.clear();
    m_cursorPos = 0;
    m_currentFile.clear();
    m_modified = false;
}
