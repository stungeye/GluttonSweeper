#pragma once

#include <memory>
#include <type_traits>

class ScreenManager;
struct GameContext;

// Base class for all screens in the game.
// 
// Screens represent different states of the application (menus, gameplay, overlays).
// The ScreenManager maintains a stack of screens and delegates Update/Draw to them.
//
// Derived classes must implement:
//   - Update() - Handle input and game logic for this screen
//   - Draw() const - Render this screen
//   - ShouldStack() const - Return true if this screen should stack on top of others
//
// Protected methods for screen transitions:
//   - RequestScreenChange<T>() - Request a transition to a new screen
//   - RequestExit() - Request that the application exit
//   - RequestClose() - Close this screen (overlays only)
//   - GetContext() - Access game-wide systems (GameManager, TextureManager, etc.)
//   - RestoreOriginalWindowSize() - Restore window to original dimensions
//
// Example of a derived screen can be seen below in FullScreen and Overlay classes.
class Screen {
private:
    std::unique_ptr<Screen> nextScreen;
    bool requestedExit;
    ScreenManager& manager;
    
    // Original window dimensions (shared across all screens)
    static int originalWindowWidth;
    static int originalWindowHeight;

protected:
    bool requestedClose;  // Protected so Overlay can set it

    // Access to game context for derived classes (implemented in Screen.cpp)
    GameContext& GetContext();
    const GameContext& GetContext() const;
    
    // Restore window to original dimensions
    void RestoreOriginalWindowSize();

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
    
    // Initialize original window dimensions (called once at startup)
    static void SetOriginalWindowDimensions(int width, int height) {
        originalWindowWidth = width;
        originalWindowHeight = height;
    }
};

// Full-screen screens replace the entire display.
// 
// Use FullScreen for major application states like:
//   - Logo/splash screens
//   - Main menu
//   - Gameplay
//   - Game over screen
//
// FullScreen automatically implements ShouldStack() to return false,
// meaning transitioning to a FullScreen will replace the current screen
// rather than stacking on top of it.
class FullScreen : public Screen {
public:
    FullScreen(ScreenManager& manager) : Screen{ manager } {}
    bool ShouldStack() const final { return false; }
};

// Overlay screens stack on top of the current screen.
// 
// Use Overlay for temporary screens that don't replace the underlying screen:
//   - Pause menu
//   - Options/settings menu
//   - Dialog boxes
//   - In-game menus
//
// Overlay automatically implements ShouldStack() to return true,
// meaning the previous screen remains in the stack and can be resumed.
// 
// Overlays can call RequestClose() to pop themselves off the stack
// and return to the previous screen.
class Overlay : public Screen {
protected:
    void RequestClose() {
        requestedClose = true;
    }

public:
    Overlay(ScreenManager& manager) : Screen{ manager } {}
    bool ShouldStack() const final { return true; }
};
