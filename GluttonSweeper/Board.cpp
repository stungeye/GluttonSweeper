#include "Board.hpp"
#include "Tile.hpp"
#include <algorithm>
#include <random>
#include <queue>

Board::Board(int w, int h, int mines)
    : width{ w }
    , height{ h }
    , mineCount{ mines }
	, flagCount{ 0 }
    , tiles(h, std::vector<Tile::TileValue>(w, Tile::UNREVEALED_EMPTY)) {
}

void Board::Initialize(std::optional<BoardPosition> safePosition) {
    // Reset all tiles to unrevealed empty
    for (auto& row : tiles) {
        for (auto& tile : row) {
            tile = Tile::UNREVEALED_EMPTY;
        }
    }

    gameOver = false;
    gameWon = false;
	flagCount = 0;
    chordedTile = std::nullopt;

    placeMines(safePosition);
    calculateAdjacentMines();
    minesPlaced = true;
    markDirty();  // Board initialized, needs rendering
}

void Board::placeMines(std::optional<BoardPosition> safePosition) {
    // Create a list of all positions
    std::vector<BoardPosition> positions;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Skip the safe position if provided
            if (safePosition.has_value() && 
                safePosition->x == x && safePosition->y == y) {
                continue;
            }
            positions.push_back({ x, y });
        }
    }

    // Shuffle and place mines in first N positions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(positions.begin(), positions.end(), gen);

    for (int i = 0; i < mineCount && i < static_cast<int>(positions.size()); ++i) {
        auto pos = positions[i];
        tiles[pos.y][pos.x] = Tile::UNREVEALED_MINE;
    }
}

void Board::calculateAdjacentMines() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!Tile::IsMine(tiles[y][x])) {
                int adjacent = countAdjacentTiles({x, y}, Tile::IsMine);
                tiles[y][x] = Tile::CreateUnrevealed(adjacent);
            }
        }
    }
}

int Board::countAdjacentTiles(BoardPosition pos, std::function<bool(Tile::TileValue)> predicate) const {
    int count = 0;
    forEachNeighbour(pos, [&](BoardPosition neighbor) {
        if (predicate(tiles[neighbor.y][neighbor.x])) {
            ++count;
        }
    });
    return count;
}

void Board::StartPreChord(BoardPosition pos) {
    if (!isValidPosition(pos) || gameOver) {
        return;
    }
    
    // Cancel any existing pre-chord first (before validation)
    if (IsPreChordActive()) {
        CancelPreChord();
    }
    
    Tile::TileValue tile = tiles[pos.y][pos.x];
    
    // Can only chord revealed tiles with adjacent mines
    if (!Tile::IsRevealed(tile) || Tile::GetAdjacentMines(tile) == 0) {
        return;
    }
    
    // Store the chorded tile position
    chordedTile = pos;
    
    // Pre-chord all unrevealed, unflagged adjacent tiles
    forEachNeighbour(pos, [&](BoardPosition neighbor) {
        Tile::TileValue& neighborTile = tiles[neighbor.y][neighbor.x];
        if (!Tile::IsRevealed(neighborTile) && !Tile::IsFlagged(neighborTile)) {
            neighborTile = Tile::PreChord(neighborTile);
			markDirty();
        }
    });
}

void Board::CancelPreChord() {
    if (!IsPreChordActive()) {
        return;  // No active chord to cancel
	}

    // Revert all pre-chorded neighbors of the chorded tile
    forEachNeighbour(*chordedTile, [&](BoardPosition neighbor) {
        Tile::TileValue& tile = tiles[neighbor.y][neighbor.x];
        if (Tile::IsPreChorded(tile)) {
            tile = Tile::UnPreChord(tile);
			markDirty();
        }
    });
    
    // Clear the chorded tile position after reverting neighbors
    chordedTile = std::nullopt;
}

bool Board::ExecuteChord() {
    if (!chordedTile.has_value()) {
        return false;  // No active chord
    }
    
    // Cache the chorded tile's position before we cancel pre-chording.
    BoardPosition pos = *chordedTile;

	// Regardless of outcome, always cancel pre-chording state (this may mark board as dirty).
    CancelPreChord();
    
    if (!isValidPosition(pos) || gameOver) {
        return false;
    }
    
    Tile::TileValue tile = tiles[pos.y][pos.x];
    
    // Can only chord revealed tiles with adjacent mines
    if (!Tile::IsRevealed(tile) || Tile::GetAdjacentMines(tile) == 0) {
        return false;
    }
    
    // Count adjacent flags
    int adjacentFlags = countAdjacentTiles(pos, Tile::IsFlagged);
    int adjacentMines = Tile::GetAdjacentMines(tile);
    
    // Chord fails if flag count doesn't match mine count
    if (adjacentFlags != adjacentMines) {
		return true; // This is considered an executed chord attempt, just no tiles revealed
    }
    
    // Reveal all adjacent unrevealed, unflagged tiles using RevealTile
    forEachNeighbour(pos, [&](BoardPosition neighbor) {
        // RevealTile handles all reveal logic, flood fill, mine detection, and win checking
        RevealTile(neighbor);
    });
    
    return true;  // Chord executed successfully
}

bool Board::RevealTile(BoardPosition pos) {
    if (!isValidPosition(pos) || gameOver) {
        return false;
    }

    Tile::TileValue& tile = tiles[pos.y][pos.x];
    
    if (Tile::IsRevealed(tile) || Tile::IsFlagged(tile)) {
        return false;
    }

    tile = Tile::Reveal(tile);
    markDirty();

    if (Tile::IsMine(tile)) {
        gameOver = true;
        return true;
    }

    // If tile has no adjacent mines, reveal adjacent tiles (flood fill)
    if (Tile::GetAdjacentMines(tile) == 0) {
        revealAdjacentTiles(pos);
    }

    checkWinCondition();
    return true;
}

void Board::revealAdjacentTiles(BoardPosition pos) {
    // Use BFS to reveal all connected empty tiles
    std::queue<BoardPosition> toProcess;
    toProcess.push(pos);

    while (!toProcess.empty()) {
        BoardPosition current = toProcess.front();
        toProcess.pop();

        forEachNeighbour(current, [&](BoardPosition neighbor) {
            Tile::TileValue& neighborTile = tiles[neighbor.y][neighbor.x];
            
            if (!Tile::IsRevealed(neighborTile) && !Tile::IsFlagged(neighborTile) && !Tile::IsMine(neighborTile)) {
                neighborTile = Tile::Reveal(neighborTile);
                markDirty();
                
                // Continue flood fill only if this tile also has no adjacent mines
                if (Tile::GetAdjacentMines(neighborTile) == 0) {
                    toProcess.push(neighbor);
                }
            }
        });
    }
}

bool Board::ToggleFlag(BoardPosition pos) {
    if (!isValidPosition(pos) || gameOver) {
        return false;
    }

    Tile::TileValue& tile = tiles[pos.y][pos.x];
    
    if (!Tile::IsRevealed(tile)) {
        if (Tile::IsFlagged(tile)) {
            tile = Tile::Unflag(tile);
			flagCount--;
        } else {
            tile = Tile::Flag(tile);
			flagCount++;
        }
        markDirty();
        return true;
    }
    
    return false;
}

Tile::TileValue Board::GetTile(BoardPosition pos) const {
    return tiles[pos.y][pos.x];
}

bool Board::isValidPosition(BoardPosition pos) const {
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

void Board::checkWinCondition() {
    // Win if all non-mine tiles are revealed
    int revealedCount = 0;
    const int totalSafeTiles = width * height - mineCount;

    for (const auto& row : tiles) {
        for (Tile::TileValue tile : row) {
            if (Tile::IsRevealed(tile) && !Tile::IsMine(tile)) {
                ++revealedCount;
            }
        }
    }

    if (revealedCount == totalSafeTiles) {
        gameWon = true;
        gameOver = true;
    }
}

void Board::revealAll() {
    for (auto& row : tiles) {
        for (Tile::TileValue& tile : row) {
			tile = Tile::Reveal(tile);
        }
    }
    markDirty();
}
