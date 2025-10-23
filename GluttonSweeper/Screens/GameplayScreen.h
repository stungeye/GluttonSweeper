#pragma once

#include "../Screen.h"

class GameplayScreen : public FullScreen {
private:
    int score;
    float timeRemaining;
    bool gameStarted;
    static constexpr float GAME_DURATION = 10.0f;

public:
    GameplayScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
