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
    int score;
    float timeRemaining;
    bool gameStarted;
    float timeSinceLastAction;
    static constexpr float GAME_DURATION{ 10.0f };
    static constexpr float IDLE_TIMEOUT{ 0.4f };

public:
    GameManager();

    // Game logic methods
    void StartGame();
    void AddScore(int points);
    void UpdateTimer(float deltaTime);
    bool IsTimeUp() const;
    bool IsRunning() const;  // Check if zombie should be running
    void Reset();

    // Getters for UI/rendering
    int GetScore() const { return score; }
    float GetTimeRemaining() const { return timeRemaining; }
    bool HasStarted() const { return gameStarted; }
    float GetGameDuration() const { return GAME_DURATION; }
};
