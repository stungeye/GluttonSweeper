#include "ScreenManager.hpp"
#include "GameManager.hpp"
#include "GameContext.hpp"
#include "TextureManager.hpp"
#include "Screens/LogoScreen.hpp"
#include <raylib.h>

int main()
{
    // Initialize window
    const int SCREEN_WIDTH{ 1280 };
    const int SCREEN_HEIGHT{ 720 };
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gluttonous Key Smasher");
    SetTargetFPS(60);
	SetExitKey(KEY_NULL);  // Disable default ESC key exit

	{ // Extra block scope so that TextureManager is destroyed before CloseWindow()
		// Create game systems
		GameManager gameManager;
		TextureManager textureManager;

		// Create context holding all game systems
		GameContext context{ gameManager, textureManager };

		// Create screen manager with context
		ScreenManager screenManager{ context };
		screenManager.SetInitialScreen<LogoScreen>("Gluttonous Key Smasher", 3.0f);

		// Main loop
		while (!WindowShouldClose() && !screenManager.ShouldExit()) {
			// Update
			screenManager.Update();

			// Draw
			BeginDrawing();
			ClearBackground(BLACK);
			screenManager.Draw();
			EndDrawing();
		}
	}

    // Cleanup
    CloseWindow();

    return 0;
}
