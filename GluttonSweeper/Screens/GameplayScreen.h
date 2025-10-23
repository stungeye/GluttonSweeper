#pragma once

#include "../Screen.h"

class GameplayScreen : public FullScreen {
public:
    GameplayScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
