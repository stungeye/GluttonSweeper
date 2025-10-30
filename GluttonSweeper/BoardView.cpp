#include "BoardView.hpp"
#include "Board.hpp"
#include "Tile.hpp"
#include <raylib.h>

BoardView::BoardView(TextureManager& tm,
                     const std::string& unrevealedPath,
                     const std::string& flaggedPath,
                     const std::string& minePath,
                     const std::string& emptyPath,
                     int size,
                     int fontSz)
    : textureManager{ tm }
    , renderTexture{}
    , tileSize{ size }
    , fontSize{ fontSz }
    , isInitialized{ false } {
	// Log Tile Size using Raylib's TraceLog
	TraceLog(LOG_INFO, "BoardView: Initializing with tile size: %d", tileSize);
    // Load tile textures
    unrevealedTile = textureManager.GetOrLoad(unrevealedPath);
    flaggedTile = textureManager.GetOrLoad(flaggedPath);
    mineTile = textureManager.GetOrLoad(minePath);
    emptyTile = textureManager.GetOrLoad(emptyPath);
}

BoardView::~BoardView() {
    unloadRenderTexture();
}

Color BoardView::getNumberColor(int number) const {
    switch (number) {
        case 1: return BLUE;
        case 2: return GREEN;
        case 3: return RED;
        case 4: return DARKBLUE;
        case 5: return MAROON;
        case 6: return SKYBLUE;
        case 7: return BLACK;
        case 8: return DARKGRAY;
        default: return BLACK;
    }
}

void BoardView::Generate(const Board& board) {
    // Unload previous render texture if it exists
    unloadRenderTexture();

    // Calculate render texture size based on board dimensions
    int width = board.GetWidth() * tileSize;
    int height = board.GetHeight() * tileSize;

    // Create new render texture
    renderTexture = LoadRenderTexture(width, height);
    isInitialized = true;

    // Begin rendering to texture
    BeginTextureMode(renderTexture);
    ClearBackground(BLANK);

    // Draw each tile based on board state
    for (int y = 0; y < board.GetHeight(); ++y) {
        for (int x = 0; x < board.GetWidth(); ++x) {
            Vector2 position = { 
                static_cast<float>(x * tileSize), 
                static_cast<float>(y * tileSize) 
            };

            Tile::TileValue tile = board.GetTile(x, y);

            if (!Tile::IsRevealed(tile)) {
                // Draw unrevealed or flagged tile
                if (Tile::IsFlagged(tile)) {
                    if (flaggedTile && flaggedTile->isValid()) {
                        DrawTextureEx(flaggedTile->raw(), position, 0.0f, 
                                     static_cast<float>(tileSize) / flaggedTile->width(), WHITE);
                    }
                } else {
                    if (unrevealedTile && unrevealedTile->isValid()) {
                        DrawTextureEx(unrevealedTile->raw(), position, 0.0f, 
                                     static_cast<float>(tileSize) / unrevealedTile->width(), WHITE);
                    }
                }
            } else {
                // Draw revealed tile
                if (Tile::IsMine(tile)) {
                    if (mineTile && mineTile->isValid()) {
                        DrawTextureEx(mineTile->raw(), position, 0.0f, 
                                     static_cast<float>(tileSize) / mineTile->width(), WHITE);
                    }
                } else {
                    int adjacentMines = Tile::GetAdjacentMines(tile);
                    
                    // Draw empty tile background
                    if (emptyTile && emptyTile->isValid()) {
                        DrawTextureEx(emptyTile->raw(), position, 0.0f, 
                                     static_cast<float>(tileSize) / emptyTile->width(), WHITE);
                    }
                    
                    // Draw number if there are adjacent mines
                    if (adjacentMines > 0) {
                        const char* numberText = TextFormat("%d", adjacentMines);
                        int textWidth = MeasureText(numberText, fontSize);
                        
                        // Center the text in the tile
                        Vector2 textPos = {
                            position.x + (tileSize - textWidth) / 2.0f,
                            position.y + (tileSize - fontSize) / 2.0f
                        };
                        
                        DrawText(numberText, 
                                static_cast<int>(textPos.x), 
                                static_cast<int>(textPos.y), 
                                fontSize, 
                                getNumberColor(adjacentMines));
                    }
                }
            }
        }
    }

    EndTextureMode();
}

void BoardView::unloadRenderTexture() {
    if (isInitialized) {
        UnloadRenderTexture(renderTexture);
        isInitialized = false;
    }
}

std::optional<std::pair<int, int>> BoardView::GetTileAtPosition(float x, float y) const {
    // Convert pixel position to tile coordinates
    const int tileX = static_cast<int>(x / tileSize);
    const int tileY = static_cast<int>(y / tileSize);

    // Calculate board dimensions from texture size
    const int boardWidth = GetWidth() / tileSize;
    const int boardHeight = GetHeight() / tileSize;

    // Check if coordinates are within board bounds
    if (tileX >= 0 && tileX < boardWidth && tileY >= 0 && tileY < boardHeight) {
        return std::make_pair(tileX, tileY);
    }

    return std::nullopt;  // Position is outside board
}
