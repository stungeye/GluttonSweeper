#pragma once

#include "Screen.h"
#include <memory>
#include <vector>

class ScreenManager {
private:
    std::vector<std::unique_ptr<Screen>> screenStack;
    bool shouldExit;

public:
    ScreenManager();
    ~ScreenManager() = default;

    void SetInitialScreen(std::unique_ptr<Screen> initialScreen);

    void Update();
    void Draw() const;

    bool ShouldExit() const { return shouldExit; }
};
