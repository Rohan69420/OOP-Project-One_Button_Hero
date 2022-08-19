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
    int HandleMenuEvent(SDL_Event &e,int unlockedLevel);
    void RenderMenu();
    void unlockedLevels(int unlockedLevel,SDL_Event &evnt);
    int MenuAction();
    void reset();
    void renderUnlockedLevel();
    int handleUnlockedLevels(SDL_Event &e,int unlockedLevel);
    void showHighScores();
    void changeLevelAndRun();
};