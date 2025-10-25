#include "GameplayScreen.hpp"
#include "MainMenuScreen.hpp"
#include "GameOverScreen.hpp"
#include "PauseOverlay.hpp"
#include "../ScreenManager.hpp"
#include "../GameContext.hpp"
#include "../GameManager.hpp"
#include <raylib.h>

GameplayScreen::GameplayScreen(ScreenManager& manager)
    : FullScreen{ manager } {
	// Load zombie textures from Assets/zombie_#.png into array:
	for (std::size_t i{ 0 }; i < zombieTextures.size(); ++i) {
		zombieTextures[i] = GetContext().textureManager.GetOrLoad(
			"Assets/zombie_" + std::to_string(i) + ".png");
	}
}

// Update is acting as the Controller and the gameManager is the Model.
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
		game.AddScore(1);  // This resets idle timer internally
		
		// Cycle through running animation frames 1, 2, 3 
		if (zombieIndex < 1 || zombieIndex >= 3) {
			zombieIndex = 1;  // Start running animation
		} else {
			zombieIndex++;    // Continue to next frame
		}
	} 
	
	// Return to idle animation (frame 0) if not running
	if (!game.IsRunning()) {
		zombieIndex = 0;
	}

	game.UpdateTimer(GetFrameTime());

	if (game.IsTimeUp()) {
		RequestScreenChange<GameOverScreen>();
	}

}

// Draw is acting as the View for the gameManager.
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

	if (zombieTextures[zombieIndex] && zombieTextures[zombieIndex]->isValid()) {
		DrawTexture(
			zombieTextures[zombieIndex]->raw(),
			0 + game.GetScore() * ZOMBIE_STEP_X,
			GetScreenHeight() - 128, WHITE);
	}
}
