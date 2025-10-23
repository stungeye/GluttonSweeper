#pragma once

#include "../Screen.hpp"
#include "../TextureManager.hpp"
#include <array>

class GameplayScreen : public FullScreen {
private:
	std::array<TextureManager::Handle, 4> zombieTextures;
	int zombieIndex{ 0 };
    static constexpr float ZOMBIE_STEP_X{ 11.0f };
	
public:
    GameplayScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
