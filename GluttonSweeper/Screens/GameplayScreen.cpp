#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "GameOverScreen.h"
#include "PauseOverlay.h"
#include "../ScreenManager.h"
#include <raylib.h>

GameplayScreen::GameplayScreen(ScreenManager& manager)
    : FullScreen{ manager }, score{ 0 }, timeRemaining{ GAME_DURATION }, gameStarted{ false } {
}

void GameplayScreen::Update() {
    // Toggle pause
    if (IsKeyPressed(KEY_P) && gameStarted) {
        RequestScreenChange<PauseOverlay>();
        return;
    }

    // Return to menu
    if (IsKeyPressed(KEY_ESCAPE) && !gameStarted) {
        RequestScreenChange<MainMenuScreen>();
        return;
    }

    // Game logic
    if (IsKeyPressed(KEY_SPACE)) {
        // Start game on first space press
        if (!gameStarted) {
            gameStarted = true;
        }
        score += 1;
    }

    // Update timer if game has started
    if (gameStarted) {
        timeRemaining -= GetFrameTime();
        
        // Check if time is up
        if (timeRemaining <= 0.0f) {
            timeRemaining = 0.0f;
            RequestScreenChange<GameOverScreen>(score);
        }
    }
}

void GameplayScreen::Draw() const {
    // Draw game background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGREEN);

    // Draw game elements
    DrawText("SMASH THAT KEY!", 50, 50, 40, YELLOW);
   
    if (!gameStarted) {
        DrawText("Press SPACE to start", 50, 100, 30, WHITE);
		DrawText("Press ESC to return to menu", 50, 150, 20, WHITE);
    } else {
        DrawText("Press SPACE to gain points", 50, 100, 30, WHITE);
		DrawText("Press P to pause", 50, 150, 20, WHITE);
    }

    DrawText(TextFormat("Score: %d", score), GetScreenWidth() / 2 - 200, 350, 100, WHITE);
    DrawText(TextFormat("Time: %.1f", timeRemaining), GetScreenWidth() / 2 - 200, 450, 100, timeRemaining < 3.0f ? RED : WHITE);
}
