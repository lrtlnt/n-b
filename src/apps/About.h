#pragma once

#include "../core/Window.h"
#include "../ui/Widget.h"
#include "../ui/Button.h"
#include <string>
#include <memory>

class About : public Widget {
public:
    About(Application* app, const std::string& title, const std::string& text);
    ~About() override;
    
    void render() override;
    void handleEvent(const Event& e) override;
    
private:
    Application* m_app;
    std::string m_text;
    std::unique_ptr<Button> m_okButton;
};
