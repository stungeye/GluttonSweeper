#pragma once

#include "../Screen.hpp"

class OptionsScreen : public Overlay {
public:
    OptionsScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
