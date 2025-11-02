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

void Board::Initialize(std::optional<std::pair<int, int>> safePosition) {
    // Reset all tiles to unrevealed empty
    for (auto& row : tiles) {
        for (auto& tile : row) {
            tile = Tile::UNREVEALED_EMPTY;
        }
    }

    gameOver = false;
    gameWon = false;
	flagCount = 0;

    placeMines(safePosition);
    calculateAdjacentMines();
}

void Board::placeMines(std::optional<std::pair<int, int>> safePosition) {
    // Create a list of all positions
    std::vector<std::pair<int, int>> positions;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Skip the safe position if provided
            if (safePosition.has_value() && 
                safePosition->first == x && safePosition->second == y) {
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
        auto [x, y] = positions[i];
        tiles[y][x] = Tile::UNREVEALED_MINE;
    }
}

void Board::calculateAdjacentMines() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!Tile::IsMine(tiles[y][x])) {
                int adjacent = countAdjacentTiles(x, y, Tile::IsMine);
                tiles[y][x] = Tile::CreateUnrevealed(adjacent);
            }
        }
    }
}

int Board::countAdjacentTiles(int x, int y, std::function<bool(Tile::TileValue)> predicate) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (isValidPosition(nx, ny) && predicate(tiles[ny][nx])) {
                ++count;
            }
        }
    }
    return count;
}

void Board::StartPreChord(int x, int y) {
    // Cancel any existing pre-chord first (before validation)
    CancelPreChord();
    
    if (!isValidPosition(x, y) || gameOver) {
        return;
    }
    
    Tile::TileValue tile = tiles[y][x];
    
    // Can only chord revealed tiles with adjacent mines
    if (!Tile::IsRevealed(tile) || Tile::GetAdjacentMines(tile) == 0) {
        return;
    }
    
    // Pre-chord all unrevealed, unflagged adjacent tiles
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (isValidPosition(nx, ny)) {
                Tile::TileValue& neighbor = tiles[ny][nx];
                if (!Tile::IsRevealed(neighbor) && !Tile::IsFlagged(neighbor)) {
                    neighbor = Tile::PreChord(neighbor);
                }
            }
        }
    }
}

void Board::CancelPreChord() {
    // Revert all pre-chorded tiles back to unrevealed
    for (auto& row : tiles) {
        for (Tile::TileValue& tile : row) {
            if (Tile::IsPreChorded(tile)) {
                tile = Tile::UnPreChord(tile);
            }
        }
    }
}

bool Board::ExecuteChord(int x, int y) {
    if (!isValidPosition(x, y) || gameOver) {
        return false;
    }
    
    Tile::TileValue tile = tiles[y][x];
    
    // Can only chord revealed tiles with adjacent mines
    if (!Tile::IsRevealed(tile) || Tile::GetAdjacentMines(tile) == 0) {
        CancelPreChord();
        return false;
    }
    
    // Count adjacent flags
    int adjacentFlags = countAdjacentTiles(x, y, Tile::IsFlagged);
    int adjacentMines = Tile::GetAdjacentMines(tile);
    
    // Chord fails if flag count doesn't match mine count
    if (adjacentFlags != adjacentMines) {
        CancelPreChord();
        return false;
    }
    
    // Cancel pre-chord state before revealing
    CancelPreChord();
    
    // Reveal all adjacent unrevealed, unflagged tiles
    bool anyRevealed = false;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (isValidPosition(nx, ny)) {
                Tile::TileValue& neighbor = tiles[ny][nx];
                
                if (!Tile::IsRevealed(neighbor) && !Tile::IsFlagged(neighbor)) {
                    neighbor = Tile::Reveal(neighbor);
                    anyRevealed = true;
                    
                    // Check if we hit a mine
                    if (Tile::IsMine(neighbor)) {
                        gameOver = true;
                    }
                    // If empty tile, trigger flood fill
                    else if (Tile::GetAdjacentMines(neighbor) == 0) {
                        revealAdjacentTiles(nx, ny);
                    }
                }
            }
        }
    }
    
    if (anyRevealed && !gameOver) {
        checkWinCondition();
    }
    
    return anyRevealed;
}

bool Board::RevealTile(int x, int y) {
    if (!isValidPosition(x, y) || gameOver) {
        return false;
    }

    Tile::TileValue& tile = tiles[y][x];
    
    if (Tile::IsRevealed(tile) || Tile::IsFlagged(tile)) {
        return false;
    }

    tile = Tile::Reveal(tile);

    if (Tile::IsMine(tile)) {
        gameOver = true;
        return true;
    }

    // If tile has no adjacent mines, reveal adjacent tiles (flood fill)
    if (Tile::GetAdjacentMines(tile) == 0) {
        revealAdjacentTiles(x, y);
    }

    checkWinCondition();
    return true;
}

void Board::revealAdjacentTiles(int x, int y) {
    // Use BFS to reveal all connected empty tiles
    std::queue<std::pair<int, int>> toProcess;
    toProcess.push({ x, y });

    while (!toProcess.empty()) {
        auto [cx, cy] = toProcess.front();
        toProcess.pop();

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;

                int nx = cx + dx;
                int ny = cy + dy;

                if (isValidPosition(nx, ny)) {
                    Tile::TileValue& neighbor = tiles[ny][nx];
                    
                    if (!Tile::IsRevealed(neighbor) && !Tile::IsFlagged(neighbor) && !Tile::IsMine(neighbor)) {
                        neighbor = Tile::Reveal(neighbor);
                        
                        // Continue flood fill only if this tile also has no adjacent mines
                        if (Tile::GetAdjacentMines(neighbor) == 0) {
                            toProcess.push({ nx, ny });
                        }
                    }
                }
            }
        }
    }
}

void Board::ToggleFlag(int x, int y) {
    if (!isValidPosition(x, y) || gameOver) {
        return;
    }

    Tile::TileValue& tile = tiles[y][x];
    
    if (!Tile::IsRevealed(tile)) {
        if (Tile::IsFlagged(tile)) {
            tile = Tile::Unflag(tile);
			flagCount--;
        } else {
            tile = Tile::Flag(tile);
			flagCount++;
        }
    }
}

Tile::TileValue Board::GetTile(int x, int y) const {
    return tiles[y][x];
}

bool Board::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
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
}
