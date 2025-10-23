#pragma once

class GameManager;
class TextureManager;

// Context holds references to all game-wide systems.
// Currently only GameManager, but could hold more in the future.
// This allows easy extension (TextureManager, AudioManager, etc.)
struct GameContext {
    GameManager& gameManager;
	TextureManager& textureManager;
    
    GameContext(GameManager& gm, TextureManager& tm) 
        : gameManager{ gm }, textureManager{ tm } {
    }
};
