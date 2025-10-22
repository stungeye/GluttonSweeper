#include "LogoScreen.h"
#include "MainMenuScreen.h"
#include "../ScreenManager.h"
#include <raylib.h>

LogoScreen::LogoScreen(ScreenManager& mgr, float duration)
    : FullScreen(mgr), displayTime(duration), elapsedTime(0.0f) {
}

void LogoScreen::Update() {
    elapsedTime += GetFrameTime();

    // Skip logo on key press
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        elapsedTime = displayTime;
    }

    // Transition when time is up
    if (elapsedTime >= displayTime) {
        RequestScreenChange<MainMenuScreen>();
    }
}

void LogoScreen::Draw() const {
    // Draw logo background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKBLUE);

    // Draw placeholder logo text
    int textWidth = MeasureText("GLUTTON SWEEPER", 40);
    DrawText("GLUTTON SWEEPER",
             GetScreenWidth() / 2 - textWidth / 2,
             GetScreenHeight() / 2 - 20,
             40,
             YELLOW);

    // Draw progress indicator
    DrawText("Press SPACE to skip", 10, GetScreenHeight() - 30, 20, WHITE);
}
