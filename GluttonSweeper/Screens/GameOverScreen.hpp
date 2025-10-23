#pragma once

#include "../Screen.hpp"

class GameOverScreen : public FullScreen {
public:
    GameOverScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
