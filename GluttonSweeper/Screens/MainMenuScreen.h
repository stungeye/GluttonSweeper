#pragma once

#include "../Screen.h"

class MainMenuScreen : public FullScreen {
private:
    int selectedOption;  // 0 = Play, 1 = Options, 2 = Exit

public:
    MainMenuScreen(ScreenManager& mgr);

    void Update() override;
    void Draw() const override;
};
