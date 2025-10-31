#pragma once

#include "../Screen.hpp"
#include <string>
#include <array>

class DifficultyOverlay : public Overlay {
private:
    std::size_t selectedOption{ 0 };
    std::array<std::string, 3> options{ "Easy", "Medium", "Hard" };
public:
    DifficultyOverlay(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
