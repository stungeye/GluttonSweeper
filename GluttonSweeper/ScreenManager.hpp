#pragma once

#include "Screen.hpp"
#include <memory>
#include <vector>
#include <type_traits>

struct GameContext;

class ScreenManager {
private:
    std::vector<std::unique_ptr<Screen>> screenStack;
    bool shouldExit;
    GameContext& context;

public:
    ScreenManager(GameContext& context);
    ~ScreenManager() = default;

    // Template method - hides unique_ptr, automatically injects 'this'
    template<typename T, typename... Args>
    void SetInitialScreen(Args&&... args) {
        static_assert(std::is_base_of_v<Screen, T>, "T must derive from Screen");
        screenStack.push_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
    }

    void Update();
    void Draw() const;

    bool ShouldExit() const { return shouldExit; }
    GameContext& GetContext() { return context; }
    const GameContext& GetContext() const { return context; }
};
