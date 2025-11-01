#include "GameplayScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../ScreenManager.hpp"
#include "../GameContext.hpp"
#include "../GameManager.hpp"
#include <raylib.h>
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
    , firstClick{ true } {
    
    // Get current monitor dimensions for proper sizing
    int currentMonitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(currentMonitor);
    int monitorHeight = GetMonitorHeight(currentMonitor);
    
    // Calculate optimal board sizing based on monitor dimensions
    BoardSizing sizing = BoardSizing::Calculate(
        boardSize.first,
        boardSize.second,
        monitorWidth,
        monitorHeight
    );

	// board Position centered in window
	boardPosition.x = sizing.tileSize;
    boardPosition.y = sizing.tileSize;
    
	// TraceLog the sizing results for debugging
	TraceLog(LOG_INFO, "Calculated Board Sizing:");
	TraceLog(LOG_INFO, " Tile Size: %d", sizing.tileSize);
	TraceLog(LOG_INFO, " Window Width: %d", sizing.windowWidth);
	TraceLog(LOG_INFO, " Window Height: %d", sizing.windowHeight);
	// Log monitor dimensions
	TraceLog(LOG_INFO, " Monitor Width: %d", monitorWidth);
	TraceLog(LOG_INFO, " Monitor Height: %d", monitorHeight);

    // Apply window sizing
    SetWindowSize(sizing.windowWidth, sizing.windowHeight);
    
    // Create BoardView with calculated tile size and appropriate font size
    int fontSize = sizing.tileSize - sizing.tileSize / 4;  // Font size scales with tile size
    boardView.emplace(
        GetContext().textureManager,
        "Assets/tile_0040.png",
        "Assets/tile_0114.png",
        "Assets/tile_0111.png",
        "Assets/tile_0000.png",
        "Assets/tileframe_9slice.png",
        sizing.tileSize,
        fontSize
    );
    
    // Initialize with empty board (no mines placed yet)
    board.Initialize(std::nullopt);
    boardView->Generate(board);
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

	// Convert screen position to board-relative position
	const Vector2 mousePos = GetMousePosition();
	const float boardX = mousePos.x - boardPosition.x;
	const float boardY = mousePos.y - boardPosition.y;

    // Handle mouse input for tile interactions
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Get tile coordinates
        if (const auto tilePos = boardView->GetTileAtPosition(boardX, boardY)) {
            const auto [tileX, tileY] = *tilePos;
            
            // If this is the first click, initialize the board with this position as safe
            if (firstClick) {
                board.Initialize(std::make_pair(tileX, tileY));
                firstClick = false;
            }
            
            if (board.RevealTile(tileX, tileY)) {
                boardView->Generate(board);  // Regenerate view on board change
            }
        }
    }

    // Right click to flag
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        // Get tile coordinates
        if (const auto tilePos = boardView->GetTileAtPosition(boardX, boardY)) {
            const auto [tileX, tileY] = *tilePos;
            
            // If this is the first interaction, initialize the board first
            // (though flagging first is unusual, we should handle it)
            if (firstClick) {
                board.Initialize(std::make_pair(tileX, tileY));
                firstClick = false;
                boardView->Generate(board);
            }
            
            board.ToggleFlag(tileX, tileY);
            boardView->Generate(board);  // Regenerate view on board change
        }
    }

    // Check for game over conditions
    if (board.IsGameOver()) {
        if (board.IsGameWon()) {
            // Player won - could show victory screen
        } else {
            // Player lost - hit a mine
            board.revealAll();
            boardView->Generate(board);
        }
    }
}

void GameplayScreen::Draw() const {
    ClearBackground(DARKGRAY);

    DrawText("Left Click: Reveal | Right Click: Flag | ESC: Menu", boardPosition.x, boardPosition.y / 2, 20, WHITE);

    // Draw the board view
    const Texture2D texture = boardView->GetTexture();
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
