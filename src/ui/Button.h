#pragma once

#include "Widget.h"

enum class ButtonType {
    Normal,
    TitleBarMin,
    TitleBarMax,
    TitleBarClose,
    StartButton
};

class Button : public Widget {
public:
    Button(const Rect& rect, const std::string& text = "", ButtonType type = ButtonType::Normal);
    
    void render() override;
    void handleEvent(const Event& e) override;
    
    void setButtonType(ButtonType type);
    ButtonType getButtonType() const;
    
private:
    ButtonType m_type;
};
