#pragma once

#include "../Screen.hpp"
#include "../Board.hpp"
#include "../BoardView.hpp"
#include "../GameManager.hpp"
#include <utility>
#include <optional>

class GameplayScreen : public FullScreen {
public:
    static constexpr int PREFERRED_TILE_SIZE = 128;
	static constexpr int OS_TASKBAR_MENUBAR_PADDING = 50; // Pixels to leave free for OS taskbar 
    static constexpr int MIN_TILE_SIZE = 16;
	static constexpr int WIDTH_PADDING_TILES = 4;  // Multiplier for tile size to calculate width padding
	static constexpr int HEIGHT_PADDING_TILES = 4; // Multiplier for tile size to calculate height padding
    
    GameplayScreen(ScreenManager& manager, std::pair<int, int> boardSize, int numberOfMines);

    void Update() override;
    void Draw() const override;

private:
	GameManager& gameManager;
    Board board;
    std::optional<BoardView> boardView;  // Use optional to allow reconstruction
    int tileSize{ PREFERRED_TILE_SIZE };
    bool firstClick;
    
    // Input handling helpers - return true if board state changed
    void handleLeftClick(BoardPosition pos);
    void handleRightClick(BoardPosition pos);
    void handleChording(const std::optional<BoardPosition>& tilePos, bool leftDown, bool rightDown);
};

// Helper struct to encapsulate board sizing calculations
struct BoardSizing {
    int tileSize;
    int windowWidth;
    int windowHeight;
    
    // Calculate optimal tile size and window dimensions for a board
    // Prefers 64px tiles but scales down if board won't fit on screen
    static BoardSizing Calculate(int boardWidth, int boardHeight, int maxRenderWidth, int maxRenderHeight);
};
