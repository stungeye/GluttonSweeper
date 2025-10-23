#include "GameOverScreen.h"
#include "MainMenuScreen.h"
#include "../ScreenManager.h"
#include <raylib.h>

GameOverScreen::GameOverScreen(ScreenManager& manager, int score)
    : FullScreen{ manager }, finalScore{ score } {
}

void GameOverScreen::Update() {
    if (IsKeyPressed(KEY_ENTER)) {
        RequestScreenChange<MainMenuScreen>();
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        RequestExit();
    }
}

void GameOverScreen::Draw() const {
    // Draw background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), MAROON);

    // Draw game over text
    int gameOverWidth = MeasureText("GAME OVER", 80);
    DrawText("GAME OVER",
             GetScreenWidth() / 2 - gameOverWidth / 2,
             100,
             80,
             YELLOW);

    // Draw final score
    const char* scoreText = TextFormat("Final Score: %d", finalScore);
    int scoreWidth = MeasureText(scoreText, 40);
    DrawText(scoreText,
             GetScreenWidth() / 2 - scoreWidth / 2,
             250,
             40,
             WHITE);

    // Draw instructions
    DrawText("Press ENTER to return to menu", 50, 400, 20, WHITE);
    DrawText("Press ESC to exit", 50, 430, 20, WHITE);
}
