#pragma once
#include "enumerators.h"
#include "Game.h"

class MainMenu { //menu class
private:
    SDL_Rect MenuSprites[4];
    SDL_Rect rectSrc;
    SDL_Rect rectDes;
    bool exit;
    int state;

public:
    MainMenu();
    void loadMenuSprites();
    void HandleMenuEvent(SDL_Event &e);
    void RenderMenu();
    bool exitCalled();
    int MenuAction();
    void reset();
};