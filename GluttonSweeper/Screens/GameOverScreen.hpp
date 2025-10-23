#pragma once

#include "../Screen.hpp"
#include "../TextureManager.hpp"

class GameOverScreen : public FullScreen {
private:
    TextureManager::Handle zombieTexture;
public:
    GameOverScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
