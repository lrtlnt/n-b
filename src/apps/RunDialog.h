#pragma once

#include "../core/Window.h"
#include "../ui/Widget.h"
#include "../ui/Button.h"
#include <string>
#include <memory>

class RunDialog : public Widget {
public:
    RunDialog(Application* app);
    ~RunDialog() override;
    
    void render() override;
    void handleEvent(const Event& e) override;
    
private:
    Application* m_app;
    std::string m_input;
    std::unique_ptr<Button> m_okButton;
    std::unique_ptr<Button> m_cancelButton;
    std::unique_ptr<Button> m_browseButton;
};
