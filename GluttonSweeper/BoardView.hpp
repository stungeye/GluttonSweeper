#pragma once

#include "TextureManager.hpp"
#include <raylib.h>
#include <string>

// Forward declaration
class Board;

// Renders a minesweeper board to a cached RenderTexture2D.
//
// BoardView generates a visual representation of the board state
// as a texture that can be drawn to the screen. Since minesweeper
// is event-driven (not real-time), the texture is only regenerated
// when explicitly requested via Generate.
//
// Usage:
//   BoardView view(textureManager, "Assets/tile.png", "Assets/flag.png", 
//                  "Assets/mine.png", "Assets/empty.png", 32);
//   view.Generate(board);  // Generate initial texture
//   
//   // In Draw():
//   DrawTexture(view.GetTexture(), x, y, WHITE);
//   
//   // After board state changes:
//   if (board.RevealTile(...)) {
//       view.Generate(board);  // Regenerate texture
//   }
//
// Numbers on revealed tiles are rendered using a font.
class BoardView {
public:
    BoardView(TextureManager& textureManager,
              const std::string& unrevealedPath,
              const std::string& flaggedPath,
              const std::string& minePath,
              const std::string& emptyPath,
              int tileSize = 32,
              int fontSize = 20);
    ~BoardView();

    // Non-copyable and non-movable (owns GPU resources)
    BoardView(const BoardView&) = delete;
    BoardView& operator=(const BoardView&) = delete;
    BoardView(BoardView&&) = delete;
    BoardView& operator=(BoardView&&) = delete;

    // Generate the board texture from current board state
    void Generate(const Board& board);

    // Get the cached texture for rendering
    Texture2D GetTexture() const { return renderTexture.texture; }

    // Get the size of the generated texture
    int GetWidth() const { return renderTexture.texture.width; }
    int GetHeight() const { return renderTexture.texture.height; }

private:
    void unloadRenderTexture();
    Color getNumberColor(int number) const;

    TextureManager& textureManager;
    RenderTexture2D renderTexture;
    int tileSize;
    int fontSize;
    bool isInitialized;

    // Cached tile sprite handles
    TextureManager::Handle unrevealedTile;
    TextureManager::Handle flaggedTile;
    TextureManager::Handle mineTile;
    TextureManager::Handle emptyTile;
};
