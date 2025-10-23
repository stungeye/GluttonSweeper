#include "Screen.h"
#include "ScreenManager.h"
#include "GameContext.h"

// GetContext implementation - needs full ScreenManager definition
GameContext& Screen::GetContext() {
    return manager.GetContext();
}

const GameContext& Screen::GetContext() const {
    return manager.GetContext();
}
