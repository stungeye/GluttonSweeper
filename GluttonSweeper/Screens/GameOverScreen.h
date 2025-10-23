#pragma once

#include "../Screen.h"

class GameOverScreen : public FullScreen {
public:
    GameOverScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
