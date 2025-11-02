#include "GameManager.hpp"
#include "raylib.h"

GameManager::GameManager()
	: timeElapsed{ 0.0f }, rightClicks{ 0 }, leftClicks{ 0 } {
}

void GameManager::Update() {
	timeElapsed += GetFrameTime();
}

void GameManager::Reset() {
	timeElapsed = 0.0f;
	rightClicks = 0;
	leftClicks = 0;
}
