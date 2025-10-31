#pragma once

// Game state manager and model for the gameplay mechanics.
//
// This GameManager class is the "Model" in an MVC pattern.
// GameplayScreen's Update() acts as the "Controller" and
// GameplayScreen's Draw() acts as the "View".
//
// The properties and behaviour of the game manager will depend
// on your game. This particular manager tracks:
//   - Player score
//   - Game timer (countdown to game over)
//   - Game state (started/not started)
//   - Player activity (for idle detection/animations)
//
// Screen can fetch a reference to the game manager like this:
//   auto& game = GetContext().gameManager;
class GameManager {
private:
    float timeElapsed;

public:
    GameManager();

    void Update();
    void Reset();
};
