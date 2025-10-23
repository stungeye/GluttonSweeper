#include "PauseOverlay.h"
#include "../ScreenManager.h"
#include <raylib.h>

PauseOverlay::PauseOverlay(ScreenManager& manager)
    : Overlay{ manager } {
}

void PauseOverlay::Update() {
    // Resume on P or ESC
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
        RequestClose();
    }
}

void PauseOverlay::Draw() const {
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 150});
    
    // Draw PAUSED text
    int pauseTextWidth{ MeasureText("PAUSED", 60) };
    DrawText("PAUSED",
             GetScreenWidth() / 2 - pauseTextWidth / 2,
             GetScreenHeight() / 2 - 30,
             60,
             YELLOW);
    
    // Draw instructions
    int instructWidth{ MeasureText("Press P or ESC to resume", 20) };
    DrawText("Press P or ESC to resume",
             GetScreenWidth() / 2 - instructWidth / 2,
             GetScreenHeight() / 2 + 50,
             20,
             WHITE);
}
