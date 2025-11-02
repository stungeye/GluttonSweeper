#pragma once

#include <cstdint>

// Tile state representation using integers with encoded state.
//
// Tiles are represented as uint8_t where:
//   - Revealed tiles: 0-9 (0 = empty, 1-8 = adjacent mines, 9 = mine)
//   - Unrevealed tiles: 10-19 (add 10 to revealed state)
//   - Flagged tiles: 20-29 (add 20 to revealed state, preserving underlying data)
//   - Pre-chorded tiles: 30-39 (add 30 to revealed state, tiles being previewed during chord)
//
// This compact representation is memory efficient and cache-friendly.
// Using uint8_t saves 75% memory compared to int (1 byte vs 4 bytes per tile).
namespace Tile {
    using TileValue = uint8_t;
    
    // Revealed states (0-9)
    constexpr TileValue EMPTY = 0;
    constexpr TileValue ONE = 1;
    constexpr TileValue TWO = 2;
    constexpr TileValue THREE = 3;
    constexpr TileValue FOUR = 4;
    constexpr TileValue FIVE = 5;
    constexpr TileValue SIX = 6;
    constexpr TileValue SEVEN = 7;
    constexpr TileValue EIGHT = 8;
    constexpr TileValue MINE = 9;
    
    // Unrevealed offset
    constexpr TileValue UNREVEALED_OFFSET = 10;
    
    // Unrevealed states (10-19)
    constexpr TileValue UNREVEALED_EMPTY = 10;
    constexpr TileValue UNREVEALED_ONE = 11;
    constexpr TileValue UNREVEALED_TWO = 12;
    constexpr TileValue UNREVEALED_THREE = 13;
    constexpr TileValue UNREVEALED_FOUR = 14;
    constexpr TileValue UNREVEALED_FIVE = 15;
    constexpr TileValue UNREVEALED_SIX = 16;
    constexpr TileValue UNREVEALED_SEVEN = 17;
    constexpr TileValue UNREVEALED_EIGHT = 18;
    constexpr TileValue UNREVEALED_MINE = 19;
    
    // Flagged offset
    constexpr TileValue FLAGGED_OFFSET = 20;
    
    // Flagged states (20-29)
    constexpr TileValue FLAGGED_EMPTY = 20;
    constexpr TileValue FLAGGED_ONE = 21;
    constexpr TileValue FLAGGED_TWO = 22;
    constexpr TileValue FLAGGED_THREE = 23;
    constexpr TileValue FLAGGED_FOUR = 24;
    constexpr TileValue FLAGGED_FIVE = 25;
    constexpr TileValue FLAGGED_SIX = 26;
    constexpr TileValue FLAGGED_SEVEN = 27;
    constexpr TileValue FLAGGED_EIGHT = 28;
    constexpr TileValue FLAGGED_MINE = 29;
    
    // Pre-chorded offset
    constexpr TileValue PRECHORD_OFFSET = 30;
    
    // Pre-chorded states (30-39)
    constexpr TileValue PRECHORD_EMPTY = 30;
    constexpr TileValue PRECHORD_ONE = 31;
    constexpr TileValue PRECHORD_TWO = 32;
    constexpr TileValue PRECHORD_THREE = 33;
    constexpr TileValue PRECHORD_FOUR = 34;
    constexpr TileValue PRECHORD_FIVE = 35;
    constexpr TileValue PRECHORD_SIX = 36;
    constexpr TileValue PRECHORD_SEVEN = 37;
    constexpr TileValue PRECHORD_EIGHT = 38;
    constexpr TileValue PRECHORD_MINE = 39;
    
    // Helper functions
    inline bool IsRevealed(TileValue tile) { 
        return tile < UNREVEALED_OFFSET; 
    }
    
    inline bool IsMine(TileValue tile) { 
        TileValue revealed = tile % UNREVEALED_OFFSET;  // Get base value (0-9)
        return revealed == MINE;
    }
    
    inline bool IsFlagged(TileValue tile) { 
        return tile >= FLAGGED_OFFSET && tile < FLAGGED_OFFSET + UNREVEALED_OFFSET;
    }
    
    inline bool IsPreChorded(TileValue tile) {
        return tile >= PRECHORD_OFFSET && tile < PRECHORD_OFFSET + UNREVEALED_OFFSET;
    }
    
    inline TileValue GetAdjacentMines(TileValue tile) {
        return tile % UNREVEALED_OFFSET;  // Works for all states
    }
    
    inline TileValue Reveal(TileValue tile) {
        if (IsRevealed(tile) || IsFlagged(tile)) {
            return tile; // Cannot reveal already revealed or flagged tiles
        }
        // Handle pre-chorded tiles
        if (IsPreChorded(tile)) {
            return tile - PRECHORD_OFFSET;  // Convert pre-chorded (30-39) to revealed (0-9)
        }
        return tile - UNREVEALED_OFFSET;  // Convert unrevealed (10-19) to revealed (0-9)
    }
    
    inline TileValue CreateUnrevealed(int adjacentMines) {
        return static_cast<TileValue>(adjacentMines + UNREVEALED_OFFSET);
    }
    
    inline TileValue Flag(TileValue tile) {
        if (IsRevealed(tile) || IsPreChorded(tile)) {
            return tile; // Cannot flag revealed or pre-chorded tiles
        }
        if (IsFlagged(tile)) {
            return tile; // Already flagged
        }
        // Convert unrevealed (10-19) to flagged (20-29)
        return tile - UNREVEALED_OFFSET + FLAGGED_OFFSET;
    }
    
    inline TileValue Unflag(TileValue tile) {
        if (!IsFlagged(tile)) {
            return tile; // Not flagged
        }
        // Convert flagged (20-29) back to unrevealed (10-19)
        return tile - FLAGGED_OFFSET + UNREVEALED_OFFSET;
    }
    
    inline TileValue PreChord(TileValue tile) {
        if (IsRevealed(tile) || IsFlagged(tile) || IsPreChorded(tile)) {
            return tile; // Cannot pre-chord revealed, flagged, or already pre-chorded tiles
        }
        // Convert unrevealed (10-19) to pre-chorded (30-39)
        return tile - UNREVEALED_OFFSET + PRECHORD_OFFSET;
    }
    
    inline TileValue UnPreChord(TileValue tile) {
        if (!IsPreChorded(tile)) {
            return tile; // Not pre-chorded
        }
        // Convert pre-chorded (30-39) back to unrevealed (10-19)
        return tile - PRECHORD_OFFSET + UNREVEALED_OFFSET;
    }
}
