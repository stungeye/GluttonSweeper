#include "MainMenuScreen.h"
#include "OptionsScreen.h"
#include "GameplayScreen.h"
#include "../ScreenManager.h"
#include <raylib.h>
#include <memory>

MainMenuScreen::MainMenuScreen(ScreenManager& manager)
    : FullScreen{ manager }, selectedOption{ 0 } {
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
                RequestScreenChange<GameplayScreen>();
                break;
            case 1:  // Options
                RequestScreenChange<OptionsScreen>();
                break;
            case 2:  // Exit
                RequestExit();
                break;
        }
    }
}

void MainMenuScreen::Draw() const {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGRAY);

    int titleWidth{ MeasureText("GLUTTONOUS KEY SMASHER", 60) };
    DrawText("GLUTTONOUS KEY SMASHER",
             GetScreenWidth() / 2 - titleWidth / 2,
             50,
             60,
             YELLOW);

	// Draw menu options
    int startY{ 200 };
    int spacing{ 80 };

    for (int i{ 0 }; i < 3; i++) {
        Color textColor{ (i == selectedOption) ? YELLOW : WHITE };
        const char* prefix{ (i == selectedOption) ? "> " : "  " };
        DrawText(prefix, 300, startY + i * spacing, 40, textColor);
        DrawText(options[i].c_str(), 350, startY + i * spacing, 40, textColor);
    }
}
