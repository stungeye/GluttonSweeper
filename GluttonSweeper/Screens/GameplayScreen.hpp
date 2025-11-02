#pragma once

#include "../Screen.hpp"
#include "../Board.hpp"
#include "../BoardView.hpp"
#include "../GameManager.hpp"
#include <utility>
#include <optional>

class GameplayScreen : public FullScreen {
public:
    static constexpr int PREFERRED_TILE_SIZE = 64;
    static constexpr int MIN_TILE_SIZE = 16;
	static constexpr int WIDTH_PADDING_TILES = 4;  // Multiplier for tile size to calculate width padding
	static constexpr int HEIGHT_PADDING_TILES = 4; // Multiplier for tile size to calculate height padding
    
    GameplayScreen(ScreenManager& manager, std::pair<int, int> boardSize, int numberOfMines);

    void Update() override;
    void Draw() const override;

private:
    Board board;
    std::optional<BoardView> boardView;  // Use optional to allow reconstruction
    Vector2 boardPosition;
    bool firstClick;
	GameManager& gameManager;
    
    // Chording state
    bool bothWerePressed{false};
    std::optional<std::pair<int, int>> lastChordTile;
    
    // Input handling helpers - return true if board state changed
    bool handleLeftClick(int tileX, int tileY);
    bool handleRightClick(int tileX, int tileY);
    bool handleChording(const std::optional<std::pair<int, int>>& tilePos, bool bothDown, bool leftDown, bool rightDown);
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
