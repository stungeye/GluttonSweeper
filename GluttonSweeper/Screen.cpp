#include "Screen.hpp"
#include "ScreenManager.hpp"
#include <raylib.h>

// Define static members
int Screen::originalWindowWidth = 0;
int Screen::originalWindowHeight = 0;

GameContext& Screen::GetContext() {
    return manager.GetContext();
}

const GameContext& Screen::GetContext() const {
    return manager.GetContext();
}

void Screen::RestoreOriginalWindowSize() {
    SetWindowSize(originalWindowWidth, originalWindowHeight);
}
