#include "GameManager.hpp"
#include "raylib.h"

GameManager::GameManager()
	: timeElapsed{ 0.0f }, flags{ 0 }, reveals{ 0 }, chords{ 0 } {
}

void GameManager::statsDebug() const {
	// Tracelog the stats:
	TraceLog(LOG_INFO, "=== GameManager Stats ===");
	TraceLog(LOG_INFO, "Time Elapsed: %d seconds", static_cast<int>(timeElapsed));
	TraceLog(LOG_INFO, "Flags Toggled: %d", flags);
	TraceLog(LOG_INFO, "Tiles Revealed: %d", reveals);
	TraceLog(LOG_INFO, "Chords Executed: %d", chords);
}

void GameManager::Update() {
	timeElapsed += GetFrameTime();
}

void GameManager::Reset() {
	timeElapsed = 0.0f;
	flags = 0;
	reveals = 0;
	chords = 0;
}
