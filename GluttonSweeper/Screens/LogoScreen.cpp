#include "LogoScreen.h"
#include "MainMenuScreen.h"
#include "../ScreenManager.h"
#include <raylib.h>

LogoScreen::LogoScreen(ScreenManager& manager, const std::string& displayMsg, float duration)
    : FullScreen{ manager }, displayMsg{ displayMsg }, displayTime{ duration }, elapsedTime{ 0.0f } {
}

void LogoScreen::Update() {
    elapsedTime += GetFrameTime();

	// Transition when time is up or when user presses SPACE
    if ((elapsedTime >= displayTime) || IsKeyPressed(KEY_SPACE)) {
        RequestScreenChange<MainMenuScreen>();
    }
}

void LogoScreen::Draw() const {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKBLUE);

    int textWidth = MeasureText(displayMsg.c_str(), 40);
    DrawText(displayMsg.c_str(),
             GetScreenWidth() / 2 - textWidth / 2,
             GetScreenHeight() / 2 - 20,
             40,
             YELLOW);

    DrawText("Press SPACE to skip", 10, GetScreenHeight() - 30, 20, WHITE);
}
