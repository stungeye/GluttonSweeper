#pragma once

#include "../Screen.h"

class GameOverScreen : public FullScreen {
private:
    int finalScore;

public:
    GameOverScreen(ScreenManager& manager, int score);

    void Update() override;
    void Draw() const override;

    void SetFinalScore(int score) { finalScore = score; }
};
