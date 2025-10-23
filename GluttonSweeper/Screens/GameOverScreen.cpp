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
    
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), MAROON);

    int gameOverWidth{ MeasureText("GAME OVER", 80) };
    DrawText("GAME OVER",
             GetScreenWidth() / 2 - gameOverWidth / 2,
             100,
             80,
             YELLOW);

    const char* scoreText{ TextFormat("Final Score: %d", game.GetScore()) };
    int scoreWidth{ MeasureText(scoreText, 40) };
    DrawText(scoreText,
             GetScreenWidth() / 2 - scoreWidth / 2,
             250,
             40,
             WHITE);

    DrawText("Press ENTER to return to menu", 50, 400, 20, WHITE);
    DrawText("Press ESC to exit", 50, 430, 20, WHITE);
}
