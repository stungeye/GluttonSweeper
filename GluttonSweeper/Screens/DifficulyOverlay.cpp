#include "DifficultyOverlay.hpp"
#include "GameplayScreen.hpp"
#include "../ScreenManager.hpp"
#include <raylib.h>

DifficultyOverlay::DifficultyOverlay(ScreenManager& manager)
    : Overlay{ manager } {
}

void DifficultyOverlay::Update() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        RequestClose();  // Signal overlay should close
        return;
    }

    // Navigate menu
    if (IsKeyPressed(KEY_UP)) {
        selectedOption = (selectedOption - 1 + options.size()) % options.size();
    }
    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption = (selectedOption + 1) % options.size();
    }

    // Select option
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        switch (selectedOption) {
        case 0:  // Easy
			RequestScreenChange<GameplayScreen>(std::make_pair(9,9), 10);
            break;
        case 1:  // Medium
			RequestScreenChange<GameplayScreen>(std::make_pair(16, 16), 40);
            break;
        case 2:  // Hard
			RequestScreenChange<GameplayScreen>(std::make_pair(30, 16), 99);
            break;
        }
    }
}

void DifficultyOverlay::Draw() const {
    // Draw semi-transparent overlay background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 150 });

    const int panelWidth{ 500 };
    const int panelHeight{ 300 };
    const int panelX{ GetScreenWidth() / 2 - panelWidth / 2 };
    const int panelY{ GetScreenHeight() / 2 - panelHeight / 2 };

    DrawRectangle(panelX, panelY, panelWidth, panelHeight, DARKBLUE);
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, YELLOW);

	const char* titleText{ "Select Difficulty" };
    const int titleWidth{ MeasureText(titleText, 40) };
	const int posX{ GetScreenWidth() / 2 - titleWidth / 2 };
    DrawText(titleText,
        posX,
        panelY + 30,
        40,
        YELLOW);

    // Draw menu options
	const int startY{ panelY + 110 };
    const int spacing{ 55 };

    for (int i{ 0 }; i < 3; i++) {
        Color textColor{ (i == selectedOption) ? YELLOW : WHITE };
        const char* prefix{ (i == selectedOption) ? "> " : "  " };
        DrawText(prefix, posX - 40, startY + i * spacing, 30, textColor);
        DrawText(options[i].c_str(), posX, startY + i * spacing, 30, textColor);
    }
}