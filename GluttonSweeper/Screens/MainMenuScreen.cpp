#include "MainMenuScreen.hpp"
#include "OptionsOverlay.hpp"
#include "DifficultyOverlay.hpp"
#include "../ScreenManager.hpp"
#include "../GameContext.hpp"
#include <raylib.h>
#include <memory>

MainMenuScreen::MainMenuScreen(ScreenManager& manager)
    : FullScreen{ manager }, selectedOption{ 0 } {
    
    // Restore original window size when returning to main menu
    RestoreOriginalWindowSize();
}

void MainMenuScreen::Update() {
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
            case 0:  // Play
                RequestScreenChange<DifficultyOverlay>();
                break;
            case 1:  // Options
                RequestScreenChange<OptionsOverlay>();
                break;
            case 2:  // Exit
                RequestExit();
                break;
        }
    }
}

void MainMenuScreen::Draw() const {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGRAY);

	const char* titleText{ "Dungeon Sweeper" };
    int titleWidth{ MeasureText(titleText, 60) };
	int posX{ GetScreenWidth() / 2 - titleWidth / 2 };
    DrawText(titleText,
             posX,
             50,
             60,
             YELLOW);

	// Draw menu options
    int startY{ 200 };
    int spacing{ 80 };

    for (int i{ 0 }; i < 3; i++) {
        Color textColor{ (i == selectedOption) ? YELLOW : WHITE };
        const char* prefix{ (i == selectedOption) ? "> " : "  " };
        DrawText(prefix, posX - 50, startY + i * spacing, 40, textColor);
        DrawText(options[i].c_str(), posX, startY + i * spacing, 40, textColor);
    }
}
