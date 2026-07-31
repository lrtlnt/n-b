#pragma once

#include "../core/Window.h"
#include "../ui/Widget.h"
#include "../ui/Button.h"
#include <string>
#include <vector>
#include <memory>

class Calculator : public Widget {
public:
    Calculator(Application* app);
    ~Calculator() override;
    
    void render() override;
    void handleEvent(const Event& e) override;
    
private:
    void appendNumber(const std::string& num);
    void setOperation(const std::string& op);
    void calculate();
    void clear();
    void clearEntry();
    void backspace();
    
    Application* m_app;
    std::string m_display;
    double m_firstNum;
    double m_secondNum;
    std::string m_operation;
    bool m_newNumber;
    
    std::vector<std::unique_ptr<Button>> m_buttons;
};
