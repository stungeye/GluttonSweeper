#pragma once

#include "../Screen.h"

class PauseOverlay : public Overlay {
public:
    PauseOverlay(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
