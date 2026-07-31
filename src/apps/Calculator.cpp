#include "Calculator.h"
#include "../core/Graphics.h"
#include "../core/Event.h"
#include <sstream>
#include <cmath>

Calculator::Calculator(Application* app)
    : Widget(Rect(0, 0, 240, 320)), m_app(app), 
      m_firstNum(0), m_secondNum(0), m_newNumber(true) {
    m_display = "0";
    
    // 创建按钮
    const std::vector<std::vector<std::string>> buttons = {
        {"Backspace", "CE", "C"},
        {"7", "8", "9", "/", "sqrt"},
        {"4", "5", "6", "*", "%"},
        {"1", "2", "3", "-", "1/x"},
        {"0", "+/-", ".", "+", "="}
    };
    
    i32 btnWidth = 40;
    i32 btnHeight = 40;
    i32 startX = 10;
    i32 startY = 70;
    i32 gap = 6;
    
    for (i32 row = 0; row < buttons.size(); row++) {
        for (i32 col = 0; col < buttons[row].size(); col++) {
            std::string text = buttons[row][col];
            i32 w = btnWidth;
            if (text == "Backspace") w = btnWidth * 2 + gap;
            
            auto btn = std::make_unique<Button>(
                Rect(startX + col * (btnWidth + gap), startY + row * (btnHeight + gap), w, btnHeight),
                text
            );
            
            std::string btnText = text;
            btn->setOnClick([this, btnText]() {
                if (btnText == "C") {
                    clear();
                } else if (btnText == "CE") {
                    clearEntry();
                } else if (btnText == "Backspace") {
                    backspace();
                } else if (btnText == "+" || btnText == "-" || btnText == "*" || btnText == "/") {
                    setOperation(btnText);
                } else if (btnText == "=") {
                    calculate();
                } else if (btnText == "sqrt") {
                    double num = std::stod(m_display);
                    std::ostringstream ss;
                    ss << sqrt(num);
                    m_display = ss.str();
                    m_newNumber = true;
                } else if (btnText == "1/x") {
                    double num = std::stod(m_display);
                    std::ostringstream ss;
                    ss << 1.0 / num;
                    m_display = ss.str();
                    m_newNumber = true;
                } else if (btnText == "%") {
                    double num = std::stod(m_display);
                    std::ostringstream ss;
                    ss << num / 100.0;
                    m_display = ss.str();
                    m_newNumber = true;
                } else if (btnText == "+/-") {
                    double num = std::stod(m_display);
                    std::ostringstream ss;
                    ss << -num;
                    m_display = ss.str();
                } else if (btnText == ".") {
                    if (m_display.find('.') == std::string::npos) {
                        m_display += ".";
                        m_newNumber = false;
                    }
                } else {
                    appendNumber(btnText);
                }
            });
            
            m_buttons.push_back(std::move(btn));
            addChild(m_buttons.back().get());
            
            if (text == "Backspace") col++;
        }
    }
}

Calculator::~Calculator() = default;

void Calculator::render() {
    if (!m_visible) return;
    
    // 背景
    Graphics::drawRect(m_rect, XPColors::ButtonFace);
    
    // 显示屏
    Rect displayRect(m_rect.x + 10, m_rect.y + 10, m_rect.w - 20, 50);
    Graphics::drawRect(displayRect, {255, 255, 255, 255});
    Graphics::drawRect(displayRect, {128, 128, 128, 255}, false);
    Graphics::drawText(m_display, displayRect.x + displayRect.w - 8, displayRect.y + 12, 
                      XPColors::TextBlack, 20, false);
    // 右对齐
    TTF_Font* font = Graphics::getFont(20);
    i32 w = 0;
    TTF_SizeText(font, m_display.c_str(), &w, nullptr);
    Graphics::drawText(m_display, displayRect.x + displayRect.w - 8 - w, displayRect.y + 12,
                      XPColors::TextBlack, 20);
    
    Widget::render();
}

void Calculator::handleEvent(const Event& e) {
    Widget::handleEvent(e);
}

void Calculator::appendNumber(const std::string& num) {
    if (m_newNumber) {
        m_display = num;
        m_newNumber = false;
    } else {
        if (m_display == "0") {
            m_display = num;
        } else {
            m_display += num;
        }
    }
}

void Calculator::setOperation(const std::string& op) {
    m_firstNum = std::stod(m_display);
    m_operation = op;
    m_newNumber = true;
}

void Calculator::calculate() {
    if (m_operation.empty()) return;
    
    m_secondNum = std::stod(m_display);
    double result = 0;
    
    if (m_operation == "+") result = m_firstNum + m_secondNum;
    else if (m_operation == "-") result = m_firstNum - m_secondNum;
    else if (m_operation == "*") result = m_firstNum * m_secondNum;
    else if (m_operation == "/") {
        if (m_secondNum != 0) result = m_firstNum / m_secondNum;
        else m_display = "除数不能为零";
    }
    
    std::ostringstream ss;
    ss << result;
    m_display = ss.str();
    m_operation.clear();
    m_newNumber = true;
}

void Calculator::clear() {
    m_display = "0";
    m_firstNum = 0;
    m_secondNum = 0;
    m_operation.clear();
    m_newNumber = true;
}

void Calculator::clearEntry() {
    m_display = "0";
    m_newNumber = true;
}

void Calculator::backspace() {
    if (m_display.size() > 1) {
        m_display.pop_back();
    } else {
        m_display = "0";
    }
}
