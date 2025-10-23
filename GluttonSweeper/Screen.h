#pragma once

#include <memory>
#include <type_traits>

class ScreenManager;

// Base for all screens
class Screen {
private:
    std::unique_ptr<Screen> nextScreen;
    bool requestedExit;
    ScreenManager& manager;

protected:
    bool requestedClose;  // Protected so Overlay can set it

    // Template method - automatically injects manager and hides unique_ptr from derived classes
	// typename T                   = type of the screen to create
    // typename... Args             = parameter pack (accepts 0 or more types)
    // Args&&...                    = forwarding reference. See: https://jaredmil.medium.com/c-move-semantics-pt-4-forwarding-references-std-forward-e32e95c72a7e 
	// std::forward<Args>(args)...  = unpack and forward each argument
    template<typename T, typename... Args>
    void RequestScreenChange(Args&&... args) {
        static_assert(std::is_base_of_v<Screen, T>, "T must derive from Screen");
        nextScreen = std::make_unique<T>(manager, std::forward<Args>(args)...);
    }

    void RequestExit() {
        requestedExit = true;
    }

public:
    Screen(ScreenManager& manager) : manager{ manager }, nextScreen{ nullptr }, requestedExit{ false }, requestedClose{ false } {}
    virtual ~Screen() = default;

    virtual void Update() = 0;
    virtual void Draw() const = 0;
    virtual bool ShouldStack() const = 0;
    
    std::unique_ptr<Screen> TakeNextScreen() {
        return std::move(nextScreen);
    }
    
    bool WantsExit() const {
        return requestedExit;
    }
    
    bool WantsClose() const {
        return requestedClose;
    }
};

// Full-screen screens (Logo, MainMenu, Gameplay, GameOver)
class FullScreen : public Screen {
public:
    FullScreen(ScreenManager& manager) : Screen{ manager } {}
    bool ShouldStack() const final { return false; }
};

// Overlays (Options, Pause, etc.)
class Overlay : public Screen {
protected:
    void RequestClose() {
        requestedClose = true;
    }

public:
    Overlay(ScreenManager& manager) : Screen{ manager } {}
    bool ShouldStack() const final { return true; }
};
