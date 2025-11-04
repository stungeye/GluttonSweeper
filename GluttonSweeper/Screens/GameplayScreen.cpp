#include "GameplayScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../ScreenManager.hpp"
#include "../GameContext.hpp"
#include <raylib.h>
#include <raymath.h>
#include <algorithm>

BoardSizing BoardSizing::Calculate(int boardWidth, int boardHeight, int maxRenderWidth, int maxRenderHeight) {
    BoardSizing result;
    
	maxRenderHeight -= GameplayScreen::OS_TASKBAR_MENUBAR_PADDING; // Leave some space for OS taskbar

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
        
        // Calculate utilization
        const float widthUtilization = (float)result.windowWidth / maxRenderWidth * 100.0f;
        const float heightUtilization = (float)result.windowHeight / maxRenderHeight * 100.0f;
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

	tileSize = sizing.tileSize;
    
    // Apply window sizing
    SetWindowSize(sizing.windowWidth, sizing.windowHeight);
    
    // Create BoardView with calculated tile size and appropriate font size
    int fontSize = tileSize - tileSize / 3;  // Font size scales with tile size
    boardView.emplace(
        GetContext().textureManager,
        "Assets/tile_0040.png",
		"Assets/tile_0116.png",    // Flagged tile
        "Assets/tile_0114.png",    // Correct flag
        "Assets/tile_0115.png",    // Incorrect flag
        "Assets/tile_0111.png",
        "Assets/tile_0000.png",
        "Assets/tileframe_9slice.png",
        tileSize,
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
        }
        return;
    }

    if (!firstClick) {
		gameManager.Update();
    }

	// Convert screen position to board-relative position and get the tile position
    const Vector2 mousePos{ GetMousePosition() };
    const Vector2 overBoardPos{ mousePos.x - tileSize, mousePos.y - tileSize};
	const auto tilePos{ boardView->GetTileFromPixelPosition(overBoardPos.x, overBoardPos.y) };

	const bool leftDown{ IsMouseButtonDown(MOUSE_LEFT_BUTTON) };
    const bool rightDown{ IsMouseButtonDown(MOUSE_RIGHT_BUTTON) };
    const bool bothDown{ leftDown && rightDown };

    // Handle chording (pre-chord, execution, cancellation)
    handleChording(tilePos, leftDown, rightDown);
    
    // Regular click handling (only if not chording)
    if (!bothDown && tilePos) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            handleLeftClick(*tilePos);
        }

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !firstClick) {
            handleRightClick(*tilePos);
        }
    }

    // Check for game over conditions
    if (board.IsGameOver()) {
		board.revealAll();
		gameManager.statsDebug();
    }
    
    // Only regenerate board view if board state changed
    if (board.IsDirty()) {
        boardView->Generate(board);
        board.ClearDirty();
    }
}

void GameplayScreen::Draw() const {
    ClearBackground(DARKGRAY);

    DrawText("Click: Reveal | Right Click: Flag | ESC: Menu", tileSize, tileSize / 4, tileSize / 2, WHITE);

    // Draw the board view
    const Texture2D texture = boardView->GetTexture();
    DrawTextureRec(texture,
        { 0, 0, (float)texture.width, -(float)texture.height },
        {(float)tileSize, (float)tileSize},
        WHITE);

    
    DrawText(TextFormat("Remaining: %d | Timer: %d", board.GetRemainingMines(), gameManager.getTimeElapsed()),
             tileSize, 
             tileSize + boardView->GetTextureHeight() + tileSize / 4, 
		     tileSize / 2, WHITE)  ;

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

void GameplayScreen::handleLeftClick(BoardPosition pos) {
    if (firstClick) {
        board.Initialize(pos);
        firstClick = false;
    }
    
    if (board.RevealTile(pos)) {
        gameManager.logReveal();
    }
}

void GameplayScreen::handleRightClick(BoardPosition pos) {
    if (board.ToggleFlag(pos)) {
        gameManager.logFlagToggle();
    }
}

void GameplayScreen::handleChording(const std::optional<BoardPosition>& tilePos, bool leftDown, bool rightDown) {
    // Pre-chord is active but mouse moved off the board - cancel it
    if (board.IsPreChordActive() && !tilePos) {
        board.CancelPreChord();
    }
    
    // Pre-chording requested (both buttons held)
    if (leftDown && rightDown && !firstClick) {
        if (tilePos) {
            // Start pre-chord or update to new tile if mouse moved
            if (board.GetChordedTile() != tilePos) {
                board.StartPreChord(*tilePos);
            }
        }
    }
    // Both buttons were released after chording - execute the chord
    else if (board.IsPreChordActive() && !leftDown && !rightDown) {
        if (board.ExecuteChord()) {
            gameManager.logChord();
        }
    }
}
