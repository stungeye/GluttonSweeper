#pragma once

#include "../Screen.hpp"
#include "../Board.hpp"
#include "../BoardView.hpp"

class GameplayScreen : public FullScreen {
private:
    Board board;
    BoardView boardView;
    Vector2 boardPosition;
    bool firstClick;
    
public:
    GameplayScreen(ScreenManager& manager, std::pair<int, int> boardSize, int numberOfMines);

    void Update() override;
    void Draw() const override;
};
