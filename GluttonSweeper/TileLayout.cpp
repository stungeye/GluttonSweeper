#include "TileLayout.hpp"

int TileLayout::snapDownToMultiple(int value, int multiple) {
    if (multiple <= 1) return value;
    return (value / multiple) * multiple;
}

TileLayoutResult TileLayout::Calculate(
    const TileGrid& grid,
    const ViewportPx& viewport,
    const PaddingTiles& padding,
    const MarginsPx& margins,
    const TileConstraints& constraints)
{
    TileLayoutResult result{};

    // Compute total tile counts including tile-based padding
    result.totalTilesWidth = grid.cols + padding.left + padding.right;
    result.totalTilesHeight = grid.rows + padding.top + padding.bottom;

    // Calculate available space after subtracting pixel margins
    const int availWidth = std::max(0, viewport.width - (margins.left + margins.right));
    const int availHeight = std::max(0, viewport.height - (margins.top + margins.bottom));

    // Try preferred tile size first
    const int preferredBoardWidth = result.totalTilesWidth * constraints.preferredTileSize;
    const int preferredBoardHeight = result.totalTilesHeight * constraints.preferredTileSize;

    int tileSize = constraints.preferredTileSize;

    // If preferred doesn't fit, compute max tile that fits both dimensions
    if (preferredBoardWidth > availWidth || preferredBoardHeight > availHeight) {
        const int maxTileByWidth = (result.totalTilesWidth > 0) 
            ? (availWidth / result.totalTilesWidth) 
            : constraints.actualTileSize;
        const int maxTileByHeight = (result.totalTilesHeight > 0) 
            ? (availHeight / result.totalTilesHeight) 
            : constraints.actualTileSize;
        
        tileSize = std::min(maxTileByWidth, maxTileByHeight);
        tileSize = snapDownToMultiple(tileSize, constraints.actualTileSize);
        
        // Clamp to minimum (max is implicitly preferredTile)
        tileSize = std::max(tileSize, constraints.actualTileSize);
    }

    // Final calculations
    result.tileSize = tileSize;
    
    // Window size is just the total tiles (no margins added to window size)
    result.windowWidth = result.totalTilesWidth * result.tileSize;
    result.windowHeight = result.totalTilesHeight * result.tileSize;

    // Board origin in pixels: tile-based padding
    result.boardOriginX = padding.left * result.tileSize;
    result.boardOriginY = padding.top * result.tileSize;

    // Derive font size (simple heuristic: 2/3 of tile size)
    result.fontSize = result.tileSize - result.tileSize / 3;

    return result;
}

MarginsPx TileLayout::DefaultMargins() {
    // Windowed mode: reserve space for Windows title bar at tope, and taskbar at bottom.
    // CSS order: top, right, bottom, left
    return MarginsPx{32, 0, 40, 0 };
}
