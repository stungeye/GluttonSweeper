#pragma once

class GameManager;

// Context holds references to all game-wide systems
// This allows easy extension (TextureManager, AudioManager, etc.)
struct GameContext {
    GameManager& gameManager;
    
    GameContext(GameManager& gm) : gameManager{ gm } {}
};
