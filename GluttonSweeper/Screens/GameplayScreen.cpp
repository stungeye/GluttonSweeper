#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "GameOverScreen.h"
#include "PauseOverlay.h"
#include "../ScreenManager.h"
#include <raylib.h>

GameplayScreen::GameplayScreen(ScreenManager& mgr)
    : FullScreen(mgr), score(0), timeRemaining(GAME_DURATION), gameStarted(false) {
}

void GameplayScreen::Update() {
    // Toggle pause
    if (IsKeyPressed(KEY_P)) {
        RequestScreenChange(std::make_unique<PauseOverlay>(manager));
        return;
    }

    // Return to menu
    if (IsKeyPressed(KEY_ESCAPE)) {
        RequestScreenChange(std::make_unique<MainMenuScreen>(manager));
        return;
    }

    // Game logic
    if (IsKeyPressed(KEY_SPACE)) {
        // Start game on first space press
        if (!gameStarted) {
            gameStarted = true;
        }
        score += 10;
    }

    // Update timer if game has started
    if (gameStarted) {
        timeRemaining -= GetFrameTime();
        
        // Check if time is up
        if (timeRemaining <= 0.0f) {
            timeRemaining = 0.0f;
            RequestScreenChange(std::make_unique<GameOverScreen>(manager, score));
        }
    }
}

void GameplayScreen::Draw() const {
    // Draw game background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGREEN);

    // Draw game elements
    DrawText("GAMEPLAY SCREEN", 50, 50, 40, YELLOW);
    
    if (!gameStarted) {
        DrawText("Press SPACE to start!", 50, 100, 30, WHITE);
    } else {
        DrawText("Press SPACE to gain points", 50, 100, 20, WHITE);
    }
    
    DrawText("Press P to pause", 50, 150, 20, WHITE);
    DrawText("Press ESC to return to menu", 50, 180, 20, WHITE);

    // Draw score
    DrawText(TextFormat("Score: %d", score), GetScreenWidth() - 700, 300, 100, WHITE);
    
    // Draw timer
    DrawText(TextFormat("Time: %.1f", timeRemaining), GetScreenWidth() - 700, 400, 100, timeRemaining < 3.0f ? RED : WHITE);
}
