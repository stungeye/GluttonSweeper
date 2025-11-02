#include "BoardView.hpp"
#include "Board.hpp"
#include "Tile.hpp"
#include <raylib.h>

constexpr int FRAME_SLICE_SIZE = 16; // Assumed constant for 9-slice scaling, adjust as needed

BoardView::BoardView(TextureManager& tm,
                     const std::string& unrevealedPath,
                     const std::string& flaggedPath,
                     const std::string& correctFlaggedPath,
                     const std::string& incorrectFlaggedPath,
                     const std::string& minePath,
                     const std::string& emptyPath,
                     const std::string& framePath,
                     int size,
                     int fontSz)
    : textureManager{ tm }
    , renderTexture{}
    , tileSize{ size }
    , fontSize{ fontSz }
    , isInitialized{ false } {
    // Load tile textures
    unrevealedTile = textureManager.GetOrLoad(unrevealedPath);
    flaggedTile = textureManager.GetOrLoad(flaggedPath);
    correctFlaggedTile = textureManager.GetOrLoad(correctFlaggedPath);
    incorrectFlaggedTile = textureManager.GetOrLoad(incorrectFlaggedPath);
    mineTile = textureManager.GetOrLoad(minePath);
    emptyTile = textureManager.GetOrLoad(emptyPath);
    frameTile = textureManager.GetOrLoad(framePath);
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

    // Calculate board size without frame
    const int boardWidth = board.GetWidth() * tileSize;
    const int boardHeight = board.GetHeight() * tileSize;
    
    // Add space for frame (one tile on each side)
    const int frameWidth = tileSize;
    const int width = boardWidth + (frameWidth * 2);
    const int height = boardHeight + (frameWidth * 2);

    // Create new render texture with space for frame
    renderTexture = LoadRenderTexture(width, height);
    isInitialized = true;

    // Begin rendering to texture
    BeginTextureMode(renderTexture);
    ClearBackground(BLANK);

    // Draw 9-slice frame around the board
    draw9SliceFrame(boardWidth, boardHeight);

    // Offset for board tiles (inside the frame)
    const int boardOffsetX = frameWidth;
    const int boardOffsetY = frameWidth;

    // Draw each tile based on board state
    const bool gameOver = board.IsGameOver();
    
    for (int y = 0; y < board.GetHeight(); ++y) {
        for (int x = 0; x < board.GetWidth(); ++x) {
            Vector2 position = { 
                static_cast<float>(boardOffsetX + x * tileSize), 
                static_cast<float>(boardOffsetY + y * tileSize) 
            };

            Tile::TileValue tile = board.GetTile(x, y);

            // Handle pre-chorded tiles (show as empty revealed)
            if (Tile::IsPreChorded(tile)) {
                if (emptyTile && emptyTile->isValid()) {
                    DrawTextureEx(emptyTile->raw(), position, 0.0f, 
                                 static_cast<float>(tileSize) / emptyTile->width(), WHITE);
                }
            }
            else if (!Tile::IsRevealed(tile)) {
                // Draw unrevealed or flagged tile
                if (Tile::IsFlagged(tile)) {
                    // Draw unrevealed background
                    if (unrevealedTile && unrevealedTile->isValid()) {
                        DrawTextureEx(unrevealedTile->raw(), position, 0.0f, 
                                     static_cast<float>(tileSize) / unrevealedTile->width(), WHITE);
                    }
                    
                    // Choose flag sprite based on game state and correctness
                    TextureManager::Handle flagSprite;
                    if (gameOver) {
                        // Game over: show correct/incorrect flags
                        bool isCorrect = Tile::IsMine(tile);
                        flagSprite = isCorrect ? correctFlaggedTile : incorrectFlaggedTile;
                    } else {
                        // Game ongoing: show regular flag
                        flagSprite = flaggedTile;
                    }
                    
                    if (flagSprite && flagSprite->isValid()) {
                        DrawTextureEx(flagSprite->raw(), position, 0.0f, 
                                     static_cast<float>(tileSize) / flagSprite->width(), WHITE);
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

void BoardView::draw9SliceFrame(int boardWidth, int boardHeight) {
    if (!frameTile || !frameTile->isValid()) {
        return;
    }

    const Texture2D& frame = frameTile->raw();
    const int scaled = tileSize;
    
    // Draw corners
    for (int row = 0; row < 3; row += 2) {  // Rows 0 and 2 (top and bottom)
        for (int col = 0; col < 3; col += 2) {  // Cols 0 and 2 (left and right)
            Rectangle source = {
                static_cast<float>(col * FRAME_SLICE_SIZE),
                static_cast<float>(row * FRAME_SLICE_SIZE),
                static_cast<float>(FRAME_SLICE_SIZE),
                static_cast<float>(FRAME_SLICE_SIZE)
            };
            
            float destX = (col == 0) ? 0.0f : static_cast<float>(scaled + boardWidth);
            float destY = (row == 0) ? 0.0f : static_cast<float>(scaled + boardHeight);
            
            Rectangle dest = { destX, destY, static_cast<float>(scaled), static_cast<float>(scaled) };
            
            DrawTexturePro(frame, source, dest, {0, 0}, 0.0f, WHITE);
        }
    }
    
    // Draw top and bottom edges (tiled)
    for (int x = scaled; x < scaled + boardWidth; x += scaled) {
        int remainingWidth = (scaled + boardWidth) - x;
        int drawWidth = (remainingWidth < scaled) ? remainingWidth : scaled;
        
        // Top edge
        DrawTexturePro(frame,
            {static_cast<float>(FRAME_SLICE_SIZE), 0, static_cast<float>(FRAME_SLICE_SIZE), static_cast<float>(FRAME_SLICE_SIZE)},
            {static_cast<float>(x), 0, static_cast<float>(drawWidth), static_cast<float>(scaled)},
            {0, 0}, 0.0f, WHITE);
        
        // Bottom edge
        DrawTexturePro(frame,
            {static_cast<float>(FRAME_SLICE_SIZE), static_cast<float>(2 * FRAME_SLICE_SIZE), static_cast<float>(FRAME_SLICE_SIZE), static_cast<float>(FRAME_SLICE_SIZE)},
            {static_cast<float>(x), static_cast<float>(scaled + boardHeight), static_cast<float>(drawWidth), static_cast<float>(scaled)},
            {0, 0}, 0.0f, WHITE);
    }
    
    // Draw left and right edges (stretched)
    // Left edge
    DrawTexturePro(frame,
        {0, static_cast<float>(FRAME_SLICE_SIZE), static_cast<float>(FRAME_SLICE_SIZE), static_cast<float>(FRAME_SLICE_SIZE)},
        {0, static_cast<float>(scaled), static_cast<float>(scaled), static_cast<float>(boardHeight)},
        {0, 0}, 0.0f, WHITE);
    
    // Right edge
    DrawTexturePro(frame,
        {static_cast<float>(2 * FRAME_SLICE_SIZE), static_cast<float>(FRAME_SLICE_SIZE), static_cast<float>(FRAME_SLICE_SIZE), static_cast<float>(FRAME_SLICE_SIZE)},
        {static_cast<float>(scaled + boardWidth), static_cast<float>(scaled), static_cast<float>(scaled), static_cast<float>(boardHeight)},
        {0, 0}, 0.0f, WHITE);
}

void BoardView::unloadRenderTexture() {
    if (isInitialized) {
        UnloadRenderTexture(renderTexture);
        isInitialized = false;
    }
}

std::optional<std::pair<int, int>> BoardView::GetTileAtPosition(float x, float y) const {
    // Account for frame offset (one tile width/height on each side)
    const int frameOffset = tileSize;
    const float boardX = x - frameOffset;
    const float boardY = y - frameOffset;
    
    // Convert pixel position to tile coordinates
    const int tileX = static_cast<int>(boardX / tileSize);
    const int tileY = static_cast<int>(boardY / tileSize);

    // Calculate board dimensions from texture size (subtract frame space)
    const int boardWidth = (GetTextureWidth() - (frameOffset * 2)) / tileSize;
    const int boardHeight = (GetTextureHeight() - (frameOffset * 2)) / tileSize;

    // Check if coordinates are within board bounds
    if (tileX >= 0 && tileX < boardWidth && tileY >= 0 && tileY < boardHeight) {
        return std::make_pair(tileX, tileY);
    }

    return std::nullopt;  // Position is outside board
}
