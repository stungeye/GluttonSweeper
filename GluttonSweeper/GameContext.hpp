#pragma once

#include "ResourceManager.hpp"

class GameManager;

// Context holds references to all game-wide systems.
// Currently only GameManager and TextureManager, but could hold 
// more in the future. (SoundManager, FontManager, etc.)
//
// Screens can access the context via their GetContext().
// This avoids the need to pass multiple references around.
//
// GameContext is a simple struct with public members.
//
// Example of a Screen accessing the gameManager via context:
//   auto& context = GetContext();
//   context.gameManager.AddScore(10);
// 
// Or more concisely:
//   GetContext().gameManager.AddScore(10);
struct GameContext {
    GameManager& gameManager;
    TextureManager& textureManager;
	SoundManager& soundManager;
    
    GameContext(GameManager& gm, TextureManager& tm, SoundManager& sm) 
		: gameManager{ gm }, textureManager{ tm }, soundManager{ sm } { }

    // Non-copyable (and non-assignable)
    GameContext(const GameContext&) = delete;
    GameContext& operator=(const GameContext&) = delete;

    // Also non-movable to avoid rebinding references implicitly
    GameContext(GameContext&&) = delete;
    GameContext& operator=(GameContext&&) = delete;
};
