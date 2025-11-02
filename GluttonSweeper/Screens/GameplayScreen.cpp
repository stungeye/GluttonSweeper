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

    TraceLog(LOG_INFO, "=== BoardSizing::Calculate ===");
    TraceLog(LOG_INFO, "Input - Board dimensions: %dx%d tiles", boardWidth, boardHeight);
    TraceLog(LOG_INFO, "Input - Max render area: %dx%d pixels", maxRenderWidth, maxRenderHeight);
    // Try preferred tile size first
	const int totalTilesWidth = boardWidth + GameplayScreen::WIDTH_PADDING_TILES;
	const int totalTilesHeight = boardHeight + GameplayScreen::HEIGHT_PADDING_TILES;
    TraceLog(LOG_INFO, "Total tiles (including padding): %dx%d", totalTilesWidth, totalTilesHeight);
    
    const int preferredBoardWidth = totalTilesWidth * GameplayScreen::PREFERRED_TILE_SIZE;
    const int preferredBoardHeight = totalTilesHeight * GameplayScreen::PREFERRED_TILE_SIZE;
    TraceLog(LOG_INFO, "Preferred board size (%d px tiles): %dx%d pixels", 
             GameplayScreen::PREFERRED_TILE_SIZE, preferredBoardWidth, preferredBoardHeight);
    
    if (preferredBoardWidth <= maxRenderWidth && preferredBoardHeight <= maxRenderHeight) {
        // Board fits with preferred tile size - use it
        TraceLog(LOG_INFO, "✓ Board FITS with preferred tile size");
        result.tileSize = GameplayScreen::PREFERRED_TILE_SIZE;
        result.windowWidth = preferredBoardWidth;
        result.windowHeight = preferredBoardHeight;
        TraceLog(LOG_INFO, "Result - Tile size: %d px", result.tileSize);
        TraceLog(LOG_INFO, "Result - Window size: %dx%d", result.windowWidth, result.windowHeight);
    } else {
        // Board too large - calculate tile size that fits within screen (including padding at that size)
        TraceLog(LOG_WARNING, "✗ Board TOO LARGE for preferred tile size");
        TraceLog(LOG_INFO, "Overflow - Width: %d px (%d over), Height: %d px (%d over)",
                 preferredBoardWidth, preferredBoardWidth - maxRenderWidth,
                 preferredBoardHeight, preferredBoardHeight - maxRenderHeight);
        
        const int maxTileSizeForWidth = maxRenderWidth / totalTilesWidth;
        const int maxTileSizeForHeight = maxRenderHeight / totalTilesHeight;
        TraceLog(LOG_INFO, "Max tile size constrained by width: %d px", maxTileSizeForWidth);
        TraceLog(LOG_INFO, "Max tile size constrained by height: %d px", maxTileSizeForHeight);
        
        // Use the smaller of the two to ensure board fits in both dimensions
        const int calculatedTileSize = std::min(maxTileSizeForWidth, maxTileSizeForHeight);
        TraceLog(LOG_INFO, "Calculated tile size (before rounding): %d px", calculatedTileSize);
        
        // Round down to nearest multiple of 16 for clean sprite scaling
        result.tileSize = (calculatedTileSize / 16) * 16;
        TraceLog(LOG_INFO, "Tile size after rounding to multiple of 16: %d px", result.tileSize);
        
        result.tileSize = std::max(result.tileSize, GameplayScreen::MIN_TILE_SIZE); // Clamp to minimum
        if (result.tileSize == GameplayScreen::MIN_TILE_SIZE) {
            TraceLog(LOG_WARNING, "⚠ Tile size clamped to minimum: %d px", GameplayScreen::MIN_TILE_SIZE);
        }
        
        // Calculate window size with padding at the calculated tile size
		result.windowWidth = result.tileSize * totalTilesWidth;
        result.windowHeight = result.tileSize * totalTilesHeight;
        TraceLog(LOG_INFO, "Result - Tile size: %d px", result.tileSize);
        TraceLog(LOG_INFO, "Result - Window size: %dx%d", result.windowWidth, result.windowHeight);
        
        // Calculate utilization
        const float widthUtilization = (float)result.windowWidth / maxRenderWidth * 100.0f;
        const float heightUtilization = (float)result.windowHeight / maxRenderHeight * 100.0f;
        TraceLog(LOG_INFO, "Screen utilization - Width: %.1f%%, Height: %.1f%%", 
                 widthUtilization, heightUtilization);
    }
    
    TraceLog(LOG_INFO, "=== End BoardSizing::Calculate ===");
    
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
    const Vector2 mousePos{ GetMousePosition() };
    const Vector2 overBoardPos{ Vector2Subtract(mousePos,boardPosition) };
	const auto tilePos{ boardView->GetTileAtPosition(overBoardPos.x, overBoardPos.y) };

	const bool leftDown{ IsMouseButtonDown(MOUSE_LEFT_BUTTON) };
    const bool rightDown{ IsMouseButtonDown(MOUSE_RIGHT_BUTTON) };
    const bool bothDown{ leftDown && rightDown };


    bool boardChanged{ false };

    // Handle chording (pre-chord, execution, cancellation)
    if (handleChording(tilePos, bothDown, leftDown, rightDown)) {
        boardChanged = true;
    }
    
    // Regular click handling (only if not chording)
    if (!bothWerePressed && tilePos) {
        const auto [tileX, tileY]{ *tilePos };

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (handleLeftClick(tileX, tileY)) {
                boardChanged = true;
            }
        }

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !firstClick) {
            if (handleRightClick(tileX, tileY)) {
                boardChanged = true;
            }
        }
    }

    // Check for game over conditions
    if (board.IsGameOver()) {
		board.revealAll();
		boardChanged = true;
    }
    
    // Only regenerate board view if something changed
    if (boardChanged) {
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

bool GameplayScreen::handleLeftClick(int tileX, int tileY) {
    gameManager.logLeftClick();
    
    if (firstClick) {
        board.Initialize(std::make_pair(tileX, tileY));
        firstClick = false;
    }
    
    return board.RevealTile(tileX, tileY);
}

bool GameplayScreen::handleRightClick(int tileX, int tileY) {
    gameManager.logRightClick();
    return board.ToggleFlag(tileX, tileY);
}

bool GameplayScreen::handleChording(const std::optional<std::pair<int, int>>& tilePos, bool bothDown, bool leftDown, bool rightDown) {
    bool boardChanged = false;
    
    // Pre-chord is active but mouse moved off the tile - cancel it
    if (lastChordTile.has_value() && lastChordTile != tilePos) {
        board.CancelPreChord();
        lastChordTile = std::nullopt;
        bothWerePressed = false;
        boardChanged = true;
    }
    
    // Chording logic (both buttons held)
    if (bothDown && !firstClick) {
        if (tilePos) {
            // Start pre-chord or update to new tile if mouse moved
            if (!lastChordTile.has_value() || lastChordTile != tilePos) {
                const auto [tileX, tileY] = *tilePos;
                board.StartPreChord(tileX, tileY);
                lastChordTile = tilePos;
                boardChanged = true;
            }
        }
        bothWerePressed = true;
    }
    // Both buttons were pressed and now both are released - execute the chord
    else if (bothWerePressed && !leftDown && !rightDown) {
        if (lastChordTile.has_value()) {
            const auto [chordX, chordY] = *lastChordTile;
            board.ExecuteChord(chordX, chordY);
            boardChanged = true;
        }
        // Reset state when both buttons are released
        bothWerePressed = false;
        lastChordTile = std::nullopt;
    }
    
    return boardChanged;
}
