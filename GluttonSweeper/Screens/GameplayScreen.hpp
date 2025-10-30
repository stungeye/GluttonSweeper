#pragma once

#include "../Screen.hpp"
#include "../Board.hpp"
#include "../BoardView.hpp"

class GameplayScreen : public FullScreen {
private:
    Board board;
    BoardView boardView;
    Vector2 boardPosition;
    
public:
    GameplayScreen(ScreenManager& manager);

    void Update() override;
    void Draw() const override;
};
