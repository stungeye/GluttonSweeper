#include "GameplayScreen.hpp"
#include "MainMenuScreen.hpp"
#include "GameOverScreen.hpp"
#include "../ScreenManager.hpp"
#include "../GameContext.hpp"
#include "../GameManager.hpp"
#include <raylib.h>

GameplayScreen::GameplayScreen(ScreenManager& manager)
    : FullScreen{ manager }
    , board{ 9, 9, 10 }  // 10x10 board with 15 mines
    , boardView{ 
        GetContext().textureManager,
        "Assets/tile_0040.png",
        "Assets/tile_0114.png",
        "Assets/tile_0111.png",
        "Assets/tile_0000.png",
        64,  // Tile size in pixels
        60   // Font size for numbers
    }
    , boardPosition{ 50.0f, 150.0f }
    , firstClick{ true } {
    
    // Initialize with empty board (no mines placed yet)
    board.Initialize(std::nullopt);
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
            firstClick = true;
            board.Initialize(std::nullopt);
            boardView.Generate(board);
        }
        return;
    }

    // Handle mouse input for tile interactions
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        
        // Convert screen position to board-relative position
        float boardX = mousePos.x - boardPosition.x;
        float boardY = mousePos.y - boardPosition.y;
        
        // Get tile coordinates
        if (auto tilePos = boardView.GetTileAtPosition(boardX, boardY)) {
            auto [tileX, tileY] = *tilePos;
            
            // If this is the first click, initialize the board with this position as safe
            if (firstClick) {
                board.Initialize(std::make_pair(tileX, tileY));
                firstClick = false;
            }
            
            if (board.RevealTile(tileX, tileY)) {
                boardView.Generate(board);  // Regenerate view on board change
            }
        }
    }

    // Right click to flag
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        
        // Convert screen position to board-relative position
        float boardX = mousePos.x - boardPosition.x;
        float boardY = mousePos.y - boardPosition.y;
        
        // Get tile coordinates
        if (auto tilePos = boardView.GetTileAtPosition(boardX, boardY)) {
            auto [tileX, tileY] = *tilePos;
            
            // If this is the first interaction, initialize the board first
            // (though flagging first is unusual, we should handle it)
            if (firstClick) {
                board.Initialize(std::make_pair(tileX, tileY));
                firstClick = false;
                boardView.Generate(board);
            }
            
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
