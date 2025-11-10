#pragma once

#include "../Screen.hpp"
#include "../Board.hpp"
#include "../BoardView.hpp"
#include "../GameManager.hpp"
#include "../TileLayout.hpp"
#include <utility>
#include <optional>

class GameplayScreen : public FullScreen {
public:
    static constexpr int PREFERRED_TILE_SIZE = 128;
    static constexpr int ACTUAL_TILE_SIZE = 16;
    
    GameplayScreen(ScreenManager& manager, TileGrid boardSize, int numberOfMines);

    void Update() override;
    void Draw() const override;

private:
	GameManager& gameManager;
    Board board;
    std::optional<BoardView> boardView;
    TileLayoutResult layout;  // Store layout result for use in Update/Draw
    
    // Input handling helpers
    void handleLeftClick(BoardPosition pos);
    void handleRightClick(BoardPosition pos);
    void handleChording(const std::optional<BoardPosition>& tilePos, bool leftDown, bool rightDown);
};
