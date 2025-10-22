#include "ScreenManager.h"

ScreenManager::ScreenManager() : shouldExit(false) {
}

void ScreenManager::Update() {
    if (screenStack.empty()) return;

	Screen& topScreen = *screenStack.back();

    topScreen.Update();

    // Check if top screen wants to close
    if (topScreen.WantsClose()) {
        screenStack.pop_back();
    }
    // Check if top screen wants to exit
    else if (topScreen.WantsExit()) {
        shouldExit = true;
    }
    // Check if top screen wants to transition
    else if (auto nextScreen = topScreen.TakeNextScreen()) {
        if (!nextScreen->ShouldStack()) {
            // Replace all screens (full screen behavior)
            screenStack.clear();
        }
		screenStack.push_back(std::move(nextScreen));
    }
}

void ScreenManager::Draw() const {
    // Draw all screens bottom-to-top
    for (const auto& screen : screenStack) {
        screen->Draw();
    }
}
