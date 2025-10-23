#pragma once

class GameManager;

// Context holds references to all game-wide systems.
// Currently only GameManager, but could hold more in the future.
// This allows easy extension (TextureManager, AudioManager, etc.)
struct GameContext {
    GameManager& gameManager;
    
    GameContext(GameManager& gm) : gameManager{ gm } {}
};
