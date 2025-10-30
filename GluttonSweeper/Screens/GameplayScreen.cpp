#include "GameplayScreen.hpp"
#include "MainMenuScreen.hpp"
#include "GameOverScreen.hpp"
#include "../ScreenManager.hpp"
#include "../GameContext.hpp"
#include "../GameManager.hpp"
#include <raylib.h>

GameplayScreen::GameplayScreen(ScreenManager& manager)
    : FullScreen{ manager }
    , board{ 9, 9, 10 }  
    , boardView{ 
        GetContext().textureManager,
        "Assets/tile_0040.png",
        "Assets/tile_0114.png",
        "Assets/tile_0111.png",
        "Assets/tile_0000.png",
        64,  // Tile size in pixels
        50   // Font size for numbers
    }
    , boardPosition{ 50.0f, 150.0f } {
    
    // Initialize the board and generate the initial view
    board.Initialize();
    boardView.Generate(board);
}

void GameplayScreen::Update() {
    // ESC returns to main menu
    if (IsKeyPressed(KEY_ESCAPE)) {
        RequestScreenChange<MainMenuScreen>();
        return;
    }

    // Don't process input if game is over
    if (board.IsGameOver()) {
        if (IsKeyPressed(KEY_SPACE)) {
            // Restart game
            board.Initialize();
            boardView.Generate(board);
        }
        return;
    }

    // Handle mouse input for tile interactions
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        
        // Check if click is within board bounds
        int tileX = static_cast<int>((mousePos.x - boardPosition.x) / 64);
        int tileY = static_cast<int>((mousePos.y - boardPosition.y) / 64);
        
        if (tileX >= 0 && tileX < board.GetWidth() && 
            tileY >= 0 && tileY < board.GetHeight()) {
            if (board.RevealTile(tileX, tileY)) {
                boardView.Generate(board);  // Regenerate view on board change
            }
        }
    }

    // Right click to flag
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        
        int tileX = static_cast<int>((mousePos.x - boardPosition.x) / 64);
        int tileY = static_cast<int>((mousePos.y - boardPosition.y) / 64);
        
        if (tileX >= 0 && tileX < board.GetWidth() && 
            tileY >= 0 && tileY < board.GetHeight()) {
            board.ToggleFlag(tileX, tileY);
            boardView.Generate(board);  // Regenerate view on board change
        }
    }

    // Check for game over conditions
    if (board.IsGameOver()) {
        if (board.IsGameWon()) {
            // Player won - could show victory screen
        } else {
            // Player lost - hit a mine
            board.revealAll();
			boardView.Generate(board);
        }
    }
}

void GameplayScreen::Draw() const {
    ClearBackground(DARKGRAY);

    DrawText("Dungeon Sweeper", 50, 50, 40, YELLOW);
    DrawText("Left Click: Reveal | Right Click: Flag | ESC: Menu", 50, 100, 20, WHITE);

    // Draw the board view
	Texture2D texture = boardView.GetTexture();
    DrawTextureRec(texture,
                { 0, 0, (float)texture.width, -(float)texture.height },
                boardPosition,
                WHITE);

    // Draw game state information
    if (board.IsGameOver()) {
        if (board.IsGameWon()) {
            DrawText("YOU WIN!", GetScreenWidth() / 2 - 100, 
                    GetScreenHeight() - 100, 40, GREEN);
        } else {
            DrawText("GAME OVER!", GetScreenWidth() / 2 - 120, 
                    GetScreenHeight() - 100, 40, RED);
        }
        DrawText("Press SPACE to restart", GetScreenWidth() / 2 - 150, 
                GetScreenHeight() - 50, 20, WHITE);
    }
}
