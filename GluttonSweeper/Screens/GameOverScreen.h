#pragma once

#include "../Screen.h"

class GameOverScreen : public FullScreen {
private:
    int finalScore;

public:
    GameOverScreen(ScreenManager& mgr, int finalScore);

    void Update() override;
    void Draw() const override;
};
