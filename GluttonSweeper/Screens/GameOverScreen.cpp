#include "GameOverScreen.h"
#include "MainMenuScreen.h"
#include "../ScreenManager.h"
#include "../GameContext.h"
#include "../GameManager.h"
#include <raylib.h>

GameOverScreen::GameOverScreen(ScreenManager& manager)
    : FullScreen{ manager } {
}

void GameOverScreen::Update() {
    if (IsKeyPressed(KEY_ENTER)) {
        // Reset game state for next play
        GetContext().gameManager.Reset();
        RequestScreenChange<MainMenuScreen>();
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        RequestExit();
    }
}

void GameOverScreen::Draw() const {
    auto& game = GetContext().gameManager;
    
    // Draw background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), MAROON);

    // Draw game over text
    int gameOverWidth{ MeasureText("GAME OVER", 80) };
    DrawText("GAME OVER",
             GetScreenWidth() / 2 - gameOverWidth / 2,
             100,
             80,
             YELLOW);

    // Draw final score from GameManager
    const char* scoreText{ TextFormat("Final Score: %d", game.GetScore()) };
    int scoreWidth{ MeasureText(scoreText, 40) };
    DrawText(scoreText,
             GetScreenWidth() / 2 - scoreWidth / 2,
             250,
             40,
             WHITE);

    // Draw instructions
    DrawText("Press ENTER to return to menu", 50, 400, 20, WHITE);
    DrawText("Press ESC to exit", 50, 430, 20, WHITE);
}
