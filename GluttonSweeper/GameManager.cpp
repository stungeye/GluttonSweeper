#include "GameManager.hpp"
#include "raylib.h"

GameManager::GameManager()
    : timeElapsed{ 0.0f } {
}

void GameManager::Update() {
	timeElapsed += GetFrameTime();
}

void GameManager::Reset() {
	timeElapsed = 0.0f;
}
