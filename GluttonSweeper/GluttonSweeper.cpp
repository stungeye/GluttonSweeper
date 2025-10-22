#include "ScreenManager.h"
#include "Screens/LogoScreen.h"
#include <raylib.h>

int main()
{
    // Initialize window
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Glutton Sweeper");
    SetTargetFPS(60);
	SetExitKey(KEY_NULL);  // Disable default ESC key exit

    // Create screen manager and set initial screen
    ScreenManager screenManager;
    screenManager.SetInitialScreen<LogoScreen>(3.0f);

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

    // Cleanup
    CloseWindow();

    return 0;
}
