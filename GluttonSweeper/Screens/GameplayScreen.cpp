#include "GameplayScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../ScreenManager.hpp"
#include "../GameContext.hpp"
#include <raylib.h>
#include <raymath.h>
#include <algorithm>

GameplayScreen::GameplayScreen(ScreenManager& manager, TileGrid boardSize, int numberOfMines)
    : FullScreen{ manager }
    , board{ boardSize.cols, boardSize.rows, numberOfMines }
	, gameManager{ manager.GetContext().gameManager } {
    
    const ViewportPx viewport = GetContext().displayService.GetViewportPx();
    
    // Define tile constraints
    const TileConstraints constraints{
        PREFERRED_TILE_SIZE,  // in pixels (also the maximum)
        ACTUAL_TILE_SIZE,        // also the minimum
    };
    
    // Calculate optimal layout
    // Note: BoardView adds its own 1-tile frame around the board internally, hence the + 2.
    layout = TileLayout::Calculate(
        TileGrid{ boardSize.cols + 2, boardSize.rows + 2 },  
        viewport,
        {1, 1, 1, 1}, // Padding in tiles outside of the BoardView. 1 on each side.
		TileLayout::DefaultMargins(),
        constraints
    );
    
    // Apply window sizing
    SetWindowSize(layout.windowWidth, layout.windowHeight);
    
    // Create BoardView with calculated tile size and font size
    boardView.emplace(
        GetContext().textureManager,
        "Assets/tile_0040.png",
		"Assets/tile_0116.png",    // Flagged tile
        "Assets/tile_0114.png",    // Correct flag
        "Assets/tile_0115.png",    // Incorrect flag
        "Assets/tile_0111.png",
        "Assets/tile_0000.png",
        "Assets/tileframe_9slice.png",
        layout.tileSize,
        layout.fontSize
    );
    
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
            board.Initialize(std::nullopt);
        }
        return;
    }

    // Timer only runs after mines are placed (first click made)
    if (board.AreMinesPlaced()) {
		gameManager.Update();
    }

	// Convert screen position to board-relative position and get the tile position
    const Vector2 mousePos{ GetMousePosition() };
    const Vector2 overBoardPos{ mousePos.x - layout.boardOriginX, mousePos.y - layout.boardOriginY };
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

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && board.AreMinesPlaced()) {
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

    DrawText("Click: Reveal | Right Click: Flag | ESC: Menu", 
             layout.boardOriginX, 
             layout.boardOriginY / 4, 
             layout.tileSize / 2, 
             WHITE);

    // Draw the board view
    const Texture2D texture = boardView->GetTexture();
    DrawTextureRec(texture,
        { 0, 0, (float)texture.width, -(float)texture.height },
        { (float)layout.boardOriginX, (float)layout.boardOriginY },
        WHITE);

    DrawText(TextFormat("Remaining: %d | Timer: %d", board.GetRemainingMines(), gameManager.getTimeElapsed()),
             layout.boardOriginX, 
             layout.boardOriginY + boardView->GetTextureHeight() + layout.boardOriginY / 4, 
		     layout.tileSize / 2, 
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

void GameplayScreen::handleLeftClick(BoardPosition pos) {
    if (!board.AreMinesPlaced()) {
        board.Initialize(pos);
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
    
    // Pre-chording requested (both buttons held) - only after first click
    if (leftDown && rightDown && board.AreMinesPlaced()) {
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
