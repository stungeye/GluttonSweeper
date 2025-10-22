#pragma once

#include "../Screen.h"

class PauseOverlay : public Overlay {
public:
    PauseOverlay(ScreenManager& mgr);

    void Update() override;
    void Draw() const override;
};
