#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "GameOverScreen.h"
#include "PauseOverlay.h"
#include "../ScreenManager.h"
#include "../GameContext.h"
#include "../GameManager.h"
#include <raylib.h>

GameplayScreen::GameplayScreen(ScreenManager& manager)
    : FullScreen{ manager } {
}

void GameplayScreen::Update() {
    auto& game = GetContext().gameManager;
    
	// If the game hasn't started yet, wait for SPACE to start
	// or ESC to return to main menu.
    if (!game.HasStarted()) {
	    if (IsKeyPressed(KEY_SPACE)) {
				game.StartGame();
	    } 
		
		if (IsKeyPressed(KEY_ESCAPE)) {
			RequestScreenChange<MainMenuScreen>();
		}
        
		return; // Don't do anything else until game starts.
    }

	// If we've made it here then the game has started!
	if (IsKeyPressed(KEY_P)) {
		RequestScreenChange<PauseOverlay>();
		return;
	} 
	
	if (IsKeyPressed(KEY_SPACE)) {
		game.AddScore(1);
	}

	game.UpdateTimer(GetFrameTime());

	if (game.IsTimeUp()) {
		RequestScreenChange<GameOverScreen>();
	}
}

void GameplayScreen::Draw() const {
    const auto& game = GetContext().gameManager;
    
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGREEN);

    DrawText("GAMEPLAY SCREEN", 50, 50, 40, YELLOW);
    
    if (!game.HasStarted()) {
        DrawText("Press SPACE to start!", 50, 100, 30, WHITE);
		DrawText("Press ESC to return to menu", 50, 150, 20, WHITE);
    } else {
        DrawText("Press SPACE to gain points", 50, 100, 30, WHITE);
		DrawText("Press P to pause", 50, 150, 20, WHITE);
    }

    DrawText(TextFormat("Score: %d", game.GetScore()), GetScreenWidth() / 2 - 200, 350, 100, WHITE);
    DrawText(TextFormat("Time: %.1f", game.GetTimeRemaining()), 
             GetScreenWidth() / 2 - 200, 450, 100, 
             game.GetTimeRemaining() < 3.0f ? RED : WHITE);
}
