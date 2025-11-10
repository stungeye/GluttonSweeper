#pragma once

#include "DisplayService.hpp"
#include <algorithm>

// Tile grid dimensions (in tiles)
struct TileGrid {
    int cols;
    int rows;
};

// Padding around the grid, expressed in tiles (CSS order: top, right, bottom, left)
struct PaddingTiles {
    int top{ 1 };
    int right{ 1 };
    int bottom{ 1 };
    int left{ 1 };
};

// Pixel margins for safe areas (CSS order: top, right, bottom, left)
struct MarginsPx {
    int top{ 0 };
    int right{ 0 };
    int bottom{ 0 };
    int left{ 0 };
};

// Tile size constraints
struct TileConstraints {
    int preferredTileSize{ 128 };  // Preferred tile size (also the maximum)
    int actualTileSize{ 16 };    // Snap tile size down to nearest multiple
};

// Result of layout calculation
struct TileLayoutResult {
    int tileSize;          // Final tile size in pixels
    int windowWidth;       // Total window width in pixels
    int windowHeight;      // Total window height in pixels
    int boardOriginX;      // Pixel X where the top-left grid tile starts
    int boardOriginY;      // Pixel Y where the top-left grid tile starts
    int fontSize;          // Suggested font size for numbers/UI
    int totalTilesWidth;   // cols + left + right padding
    int totalTilesHeight;  // rows + top + bottom padding
};

// Pure tile layout calculator for grid-based UIs
class TileLayout {
public:
    // Calculate layout given inputs (pure function, no side effects)
    static TileLayoutResult Calculate(
        const TileGrid& grid,
        const ViewportPx& viewport,
        const PaddingTiles& paddingTiles,
        const MarginsPx& marginsPx,
        const TileConstraints& constraints);
    
    // Get default margins (bottom margin for Windows taskbar)
    static MarginsPx DefaultMargins();

private:
    static int snapDownToMultiple(int value, int multiple);
};
