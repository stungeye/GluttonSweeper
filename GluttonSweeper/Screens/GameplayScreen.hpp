#pragma once

#include "../Screen.hpp"

class GameplayScreen : public FullScreen {
public:
    GameplayScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
