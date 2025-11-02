#pragma once

#include "Tile.hpp"
#include <vector>
#include <optional>
#include <functional>

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

    void Initialize(std::optional<std::pair<int, int>> safePosition = std::nullopt);
    bool RevealTile(int x, int y);
    bool ToggleFlag(int x, int y);
    void revealAll();

    // Chording methods
    void StartPreChord(int x, int y);
    void CancelPreChord();
    void ExecuteChord(int x, int y);

    Tile::TileValue GetTile(int x, int y) const;
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetMineCount() const { return mineCount; }
    int GetFlagCount() const { return flagCount; }
    int GetRemainingMines() const { return mineCount - flagCount; }

    bool IsGameOver() const { return gameOver; }
    bool IsGameWon() const { return gameWon; }

private:
    void placeMines(std::optional<std::pair<int, int>> safePosition);
    void calculateAdjacentMines();
    void revealAdjacentTiles(int x, int y);
    bool isValidPosition(int x, int y) const;
    int countAdjacentTiles(int x, int y, std::function<bool(Tile::TileValue)> predicate) const;
    void checkWinCondition();

    int width;
    int height;
    int mineCount;
    int flagCount;
    std::vector<std::vector<Tile::TileValue>> tiles;
    bool gameOver{ false };
    bool gameWon{ false };
};
