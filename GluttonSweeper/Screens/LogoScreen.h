#pragma once

#include "../Screen.h"

class LogoScreen : public FullScreen {
private:
    float displayTime;
    float elapsedTime;

public:
    LogoScreen(ScreenManager& mgr, float duration = 2.0f);

    void Update() override;
    void Draw() const override;
};
