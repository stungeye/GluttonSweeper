#include "GameplayScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../ScreenManager.hpp"
#include "../GameContext.hpp"
#include <raylib.h>
#include <raymath.h>
#include <algorithm>

BoardSizing BoardSizing::Calculate(int boardWidth, int boardHeight, int maxRenderWidth, int maxRenderHeight) {
    BoardSizing result;
    
    // Try preferred tile size first
	const int totalTilesWidth = boardWidth + GameplayScreen::WIDTH_PADDING_TILES;
	const int totalTilesHeight = boardHeight + GameplayScreen::HEIGHT_PADDING_TILES;
    const int preferredBoardWidth = totalTilesWidth * GameplayScreen::PREFERRED_TILE_SIZE;
    const int preferredBoardHeight = totalTilesHeight * GameplayScreen::PREFERRED_TILE_SIZE;
    
    if (preferredBoardWidth <= maxRenderWidth && preferredBoardHeight <= maxRenderHeight) {
        // Board fits with preferred tile size - use it
        result.tileSize = GameplayScreen::PREFERRED_TILE_SIZE;
        result.windowWidth = preferredBoardWidth;
        result.windowHeight = preferredBoardHeight;
    } else {
        // Board too large - calculate tile size that fits within screen (including padding at that size)
        const int maxTileSizeForWidth = maxRenderWidth / totalTilesWidth;
        const int maxTileSizeForHeight = maxRenderHeight / totalTilesHeight;
        
        // Use the smaller of the two to ensure board fits in both dimensions
        const int calculatedTileSize = std::min(maxTileSizeForWidth, maxTileSizeForHeight);
        
        // Round down to nearest multiple of 16 for clean sprite scaling
        result.tileSize = (calculatedTileSize / 16) * 16;
        result.tileSize = std::max(result.tileSize, GameplayScreen::MIN_TILE_SIZE); // Clamp to minimum
        
        // Calculate window size with padding at the calculated tile size
		result.windowWidth = result.tileSize * totalTilesWidth;
        result.windowHeight = result.tileSize * totalTilesHeight;
    }
    
    return result;
}

GameplayScreen::GameplayScreen(ScreenManager& manager, std::pair<int, int> boardSize, int numberOfMines)
    : FullScreen{ manager }
    , board{ boardSize.first, boardSize.second, numberOfMines }
    , firstClick{ true }
	, gameManager{ manager.GetContext().gameManager } {
    
    // Get current monitor dimensions for proper sizing
    const int currentMonitor = GetCurrentMonitor();
    const int monitorWidth = GetMonitorWidth(currentMonitor);
    const int monitorHeight = GetMonitorHeight(currentMonitor);
    
    // Calculate optimal board sizing based on monitor dimensions
    const BoardSizing sizing = BoardSizing::Calculate(
        boardSize.first,
        boardSize.second,
        monitorWidth,
        monitorHeight
    );

	// board Position centered in window
	boardPosition.x = sizing.tileSize;
    boardPosition.y = sizing.tileSize;
    
    // Apply window sizing
    SetWindowSize(sizing.windowWidth, sizing.windowHeight);
    
    // Create BoardView with calculated tile size and appropriate font size
    int fontSize = sizing.tileSize - sizing.tileSize / 4;  // Font size scales with tile size
    boardView.emplace(
        GetContext().textureManager,
        "Assets/tile_0040.png",
		"Assets/tile_0116.png",    // Flagged tile
        "Assets/tile_0114.png",    // Correct flag
        "Assets/tile_0115.png",    // Incorrect flag
        "Assets/tile_0111.png",
        "Assets/tile_0000.png",
        "Assets/tileframe_9slice.png",
        sizing.tileSize,
        fontSize
    );
    
    // Initialize with empty board (no mines placed yet)
    board.Initialize(std::nullopt);
    boardView->Generate(board);
	gameManager.Reset();
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
            boardView->Generate(board);
        }
        return;
    }

    if (!firstClick) {
		gameManager.Update();
    }

	// Convert screen position to board-relative position and get the tile position
	const Vector2 mousePos = GetMousePosition();
	const Vector2 overBoardPos = Vector2Subtract(mousePos,boardPosition);
    const auto tilePos = boardView->GetTileAtPosition(overBoardPos.x, overBoardPos.y);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (tilePos) {
			gameManager.logLeftClick();

            const auto [tileX, tileY] = *tilePos;
            
            if (firstClick) {
                board.Initialize(std::make_pair(tileX, tileY));
                firstClick = false;
            }
            
            if (board.RevealTile(tileX, tileY)) {
                boardView->Generate(board);  
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !firstClick) {
        if (tilePos) {
			gameManager.logRightClick();

            const auto [tileX, tileY] = *tilePos;
            
            board.ToggleFlag(tileX, tileY);
            boardView->Generate(board);  // Regenerate view on board change
        }
    }

    // Check for game over conditions
    if (board.IsGameOver()) {
		board.revealAll();
		boardView->Generate(board);
    }
}

void GameplayScreen::Draw() const {
    ClearBackground(DARKGRAY);

    DrawText("Click: Reveal | Right Click: Flag | ESC: Menu", boardPosition.x, boardPosition.y / 4, boardPosition.y /2, WHITE);

    // Draw the board view
    const Texture2D texture = boardView->GetTexture();
    DrawTextureRec(texture,
        { 0, 0, (float)texture.width, -(float)texture.height },
        boardPosition,
        WHITE);

    
    DrawText(TextFormat("Remaining: %d | Timer: %d", board.GetRemainingMines(), gameManager.getTimeElapsed()),
             boardPosition.x, 
             boardPosition.y + boardView->GetTextureHeight() + boardPosition.y / 4, 
		     boardPosition.y / 2, WHITE)  ;

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
