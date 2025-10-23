#pragma once

class GameManager {
private:
    int score;
    float timeRemaining;
    bool gameStarted;
    static constexpr float GAME_DURATION{ 10.0f };

public:
    GameManager();

    // Game logic methods
    void StartGame();
    void AddScore(int points);
    void UpdateTimer(float deltaTime);
    bool IsTimeUp() const;
    void Reset();

    // Getters for UI/rendering
    int GetScore() const { return score; }
    float GetTimeRemaining() const { return timeRemaining; }
    bool HasStarted() const { return gameStarted; }
    float GetGameDuration() const { return GAME_DURATION; }
};
