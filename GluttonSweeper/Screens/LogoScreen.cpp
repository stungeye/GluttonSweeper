#include "LogoScreen.h"
#include "MainMenuScreen.h"
#include "../ScreenManager.h"
#include <raylib.h>

LogoScreen::LogoScreen(ScreenManager& mgr, const std::string& displayMsg, float duration)
    : FullScreen(mgr), displayMsg(displayMsg), displayTime(duration), elapsedTime(0.0f) {
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
    int textWidth = MeasureText(displayMsg.c_str(), 40);
    DrawText(displayMsg.c_str(),
             GetScreenWidth() / 2 - textWidth / 2,
             GetScreenHeight() / 2 - 20,
             40,
             YELLOW);

    // Draw progress indicator
    DrawText("Press SPACE to skip", 10, GetScreenHeight() - 30, 20, WHITE);
}
