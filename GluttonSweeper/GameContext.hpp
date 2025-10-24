#pragma once

class GameManager;


// Context holds references to all game-wide systems.
// Currently only GameManager, but could hold more in the future.
// This allows easy extension (TextureManager, AudioManager, etc.)
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
    
    GameContext(GameManager& gm) : gameManager{ gm } {}
};
