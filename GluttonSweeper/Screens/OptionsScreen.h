#pragma once

#include "../Screen.h"

class OptionsScreen : public Overlay {
public:
    OptionsScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
