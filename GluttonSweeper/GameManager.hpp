#pragma once

// This GameManager class is the "Model" for our game.
// GamePlayScreen's Update() acts as the "Controller"
// and GamePlayScreen's Draw() acts as the "View".
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
