#include "GameManager.hpp"

GameManager::GameManager()
    : score{ 0 }, timeRemaining{ GAME_DURATION }, gameStarted{ false } {
}

void GameManager::StartGame() {
    gameStarted = true;
}

void GameManager::AddScore(int points) {
    if (gameStarted) {
        score += points;
    }
}

void GameManager::UpdateTimer(float deltaTime) {
    if (gameStarted) {
        timeRemaining -= deltaTime;
        if (timeRemaining < 0.0f) {
            timeRemaining = 0.0f;
        }
    }
}

bool GameManager::IsTimeUp() const {
    return gameStarted && timeRemaining <= 0.0f;
}

void GameManager::Reset() {
    score = 0;
    timeRemaining = GAME_DURATION;
    gameStarted = false;
}
