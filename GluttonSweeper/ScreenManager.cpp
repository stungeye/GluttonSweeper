#include "ScreenManager.hpp"
#include "GameContext.hpp"

ScreenManager::ScreenManager(GameContext& context) : shouldExit{ false }, context{ context } {
}

void ScreenManager::Update() {
    if (screenStack.empty()) return;

	// screenStack.back() is the top screen
    screenStack.back()->Update();
    const bool wantsClose{ screenStack.back()->WantsClose() };
    const bool wantsExit{ screenStack.back()->WantsExit() };
	std::unique_ptr<Screen> nextScreen{ screenStack.back()->TakeNextScreen() };

    // Check if top screen wants to close
    if (wantsClose) {
        screenStack.pop_back();
        return;
    }

    // Check if top screen wants to exit
    if (wantsExit) {
        shouldExit = true;
        return;
    }

    // Check if top screen wants to transition
    if (nextScreen) {
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
