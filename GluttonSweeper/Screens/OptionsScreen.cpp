#include "OptionsScreen.h"
#include "../ScreenManager.h"
#include <raylib.h>

OptionsScreen::OptionsScreen(ScreenManager& mgr)
    : Overlay(mgr) {
}

void OptionsScreen::Update() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        RequestClose();  // Signal overlay should close
    }
}

void OptionsScreen::Draw() const {
    // Draw options panel
    int panelWidth = 400;
    int panelHeight = 300;
    int panelX = GetScreenWidth() / 2 - panelWidth / 2;
    int panelY = GetScreenHeight() / 2 - panelHeight / 2;

    DrawRectangle(panelX, panelY, panelWidth, panelHeight, DARKBLUE);
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, YELLOW);

    // Draw title
    int titleWidth = MeasureText("OPTIONS", 40);
    DrawText("OPTIONS",
             GetScreenWidth() / 2 - titleWidth / 2,
             panelY + 20,
             40,
             YELLOW);

    // Draw options (placeholder)
    DrawText("Volume: 100%", panelX + 30, panelY + 100, 20, WHITE);
    DrawText("Difficulty: Normal", panelX + 30, panelY + 150, 20, WHITE);

    // Draw instructions
    int instructWidth = MeasureText("Press ESC to close", 16);
    DrawText("Press ESC to close",
             GetScreenWidth() / 2 - instructWidth / 2,
             panelY + panelHeight - 40,
             16,
             WHITE);
}
