#pragma once

#include "../Screen.hpp"

class OptionsOverlay : public Overlay {
public:
    OptionsOverlay(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
