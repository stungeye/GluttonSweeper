#pragma once

#include "../Screen.h"

class LogoScreen : public FullScreen {
private:
    float displayTime;
    float elapsedTime;
    static constexpr float LOGO_DURATION = 3.0f;

public:
    LogoScreen(ScreenManager& mgr);

    void Update() override;
    void Draw() const override;
};
