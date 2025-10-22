#pragma once

#include <memory>

class ScreenManager;

// Base for all screens
class Screen {
private:
    std::unique_ptr<Screen> nextScreen;
    bool requestedExit;

protected:
    ScreenManager& manager;
    bool requestedClose;  // Protected so Overlay can set it

    void RequestScreenChange(std::unique_ptr<Screen> screen) {
        nextScreen = std::move(screen);
    }

    void RequestExit() {
        requestedExit = true;
    }

public:
    Screen(ScreenManager& mgr) : manager(mgr), nextScreen(nullptr), requestedExit(false), requestedClose(false) {}
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
    FullScreen(ScreenManager& mgr) : Screen(mgr) {}
    bool ShouldStack() const final { return false; }
};

// Overlays (Options, Pause, etc.)
class Overlay : public Screen {
protected:
    void RequestClose() {
        requestedClose = true;
    }

public:
    Overlay(ScreenManager& mgr) : Screen(mgr) {}
    bool ShouldStack() const final { return true; }
};
