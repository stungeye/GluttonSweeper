#pragma once

#include "Tile.hpp"
#include <vector>
#include <optional>
#include <functional>

// Represents a position on the minesweeper board
struct BoardPosition {
    int x;
    int y;
    
    bool operator==(const BoardPosition& other) const = default;
};

// Manages the minesweeper game board.
//
// The Board class handles:
//   - Board creation and initialization
//   - Mine placement
//   - Tile revealing (with flood-fill for empty tiles)
//   - Flag toggling
//   - Win/loss state detection
//
// Tiles are represented as uint8_t (see Tile.hpp for encoding).
//
// Usage:
//   Board board(10, 10, 15);  // 10x10 board with 15 mines
//   board.Initialize();  // Random mine placement
//   // Or ensure first click is safe:
//   board.Initialize({5, 5});  // No mines at (5,5)
//
//   if (board.RevealTile(x, y)) {
//       // Tile revealed successfully
//   }
//
//   board.ToggleFlag(x, y);
class Board {
public:
    Board(int width, int height, int mineCount);

    void Initialize(std::optional<BoardPosition> safePosition = std::nullopt);
    bool RevealTile(BoardPosition pos);
    bool ToggleFlag(BoardPosition pos);
    void revealAll();

    // Chording methods
    void StartPreChord(BoardPosition pos);
    void CancelPreChord();
    void ExecuteChord(BoardPosition pos);

    Tile::TileValue GetTile(BoardPosition pos) const;
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetMineCount() const { return mineCount; }
    int GetFlagCount() const { return flagCount; }
    int GetRemainingMines() const { return mineCount - flagCount; }

    bool IsGameOver() const { return gameOver; }
    bool IsGameWon() const { return gameWon; }

private:
    void placeMines(std::optional<BoardPosition> safePosition);
    void calculateAdjacentMines();
    void revealAdjacentTiles(BoardPosition pos);
    bool isValidPosition(BoardPosition pos) const;
    int countAdjacentTiles(BoardPosition pos, std::function<bool(Tile::TileValue)> predicate) const;
    void checkWinCondition();

    int width;
    int height;
    int mineCount;
    int flagCount;
    std::vector<std::vector<Tile::TileValue>> tiles;
    bool gameOver{ false };
    bool gameWon{ false };
};
