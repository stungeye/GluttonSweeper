#include "Screen.h"
#include "ScreenManager.h"
#include "GameContext.h"

/* 
   GetContext implementations.
   These couldn't be implemented in Screen.h without including 
   ScreenManager.h which would cause a circular dependency as 
   ScreenManager.h already includes Screen.h.
*/
GameContext& Screen::GetContext() {
    return manager.GetContext();
}

const GameContext& Screen::GetContext() const {
    return manager.GetContext();
}
