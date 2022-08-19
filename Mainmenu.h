#pragma once
#include "enumerators.h"
#include "Game.h"

class MainMenu { //menu class
private:
    SDL_Rect MenuSprites[4];
    SDL_Rect rectSrc, rectDes,unlockedSrc;
    bool exit,inProgress;
    int state;

public:
    MainMenu();
    void loadMenuSprites();
    void HandleMenuEvent(SDL_Event &e);
    void RenderMenu();
    void unlockedLevels(int unlockedLevel,SDL_Event &evnt);
    int MenuAction();
    void reset();
    void renderUnlockedLevel();
    void handleUnlockedLevels(SDL_Event &e);
    void showHighScores();
};