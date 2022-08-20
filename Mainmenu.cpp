#include "Mainmenu.h"
MainMenu::MainMenu(){
    reset();
	loadMenuSprites();
}
void MainMenu::reset() {
    rectDes.x = rectDes.y = 0;
    rectDes.w = 1140;
    rectDes.h = 600;
    rectSrc.x = rectSrc.y = 0;
    rectSrc.w = 1140;
    rectSrc.h = 600;

    unlockedSrc.x = unlockedSrc.y = 0; //this needed
    unlockedSrc.w = 1140;
    unlockedSrc.h = 600;
    exit = false;
    inProgress = false;
    state = 0;
}
void MainMenu::loadMenuSprites(){
    MenuSprites[0].x = 0;
    MenuSprites[0].y = 0;
    MenuSprites[0].w = screenW;
    MenuSprites[0].h = screenH;

    MenuSprites[1].x = screenW;
    MenuSprites[1].y = 0;
    MenuSprites[1].w = screenW;
    MenuSprites[1].h = screenH;

    MenuSprites[2].x = 0;
    MenuSprites[2].y = screenH;
    MenuSprites[2].w = screenW;
    MenuSprites[2].h = screenH;

    MenuSprites[3].x = screenW;
    MenuSprites[3].y = screenH;
    MenuSprites[3].w = screenW;
    MenuSprites[3].h = screenH;

}
int MainMenu::HandleMenuEvent(SDL_Event& e,int unlockedLevel) {
    if(!inProgress){
    if (e.type == SDL_MOUSEMOTION) //mouse click eent handler

            {
            if (e.button.x > 384 && e.button.x < 756 && e.button.y>110 && e.button.y < 217)
            {
                rectSrc.x = 1140; rectSrc.y = 0;
            }

            else if (e.button.x > 384 && e.button.x < 756 && e.button.y>247 && e.button.y < 354)
            {
                rectSrc.x = 0; rectSrc.y = 600;
            }

            else if (e.button.x > 384 && e.button.x < 756 && e.button.y>386 && e.button.y < 493)
            {
                rectSrc.x = 1140; rectSrc.y = 600;
                
            }
           
            else
            {
                rectSrc.x = 0; rectSrc.y = 0;
            }

            }
     if (e.type == SDL_MOUSEBUTTONDOWN) {
         if (rectSrc.x == 1140 && rectSrc.y == 600) {
             state = 3;
         }
         if (rectSrc.x == 0 && rectSrc.y == 600) {
             inProgress = true;
             state = 2;
         }
         if (rectSrc.x == 1140 && rectSrc.y == 0) {
             state = 1;
         }
     }
         RenderMenu();
         return 0;
     }
    else {
       return handleUnlockedLevels(e,unlockedLevel);
    }
}
int MainMenu::MenuAction() {
    return state;
}
void MainMenu::unlockedLevels(int progress,SDL_Event &evnt) {
    switch (progress)
    {
    case 1: //no states saved
        unlockedSrc.x = 0; unlockedSrc.y = 0;
        break;
    case 2: //reached level 2, progress will be changed to 1
        unlockedSrc.x = 0; unlockedSrc.y = 600;
        break;
    case 3: // completed level2, progress value should be changed to 2
        unlockedSrc.x = 0; unlockedSrc.y = 1200;
        break;
    }
    renderUnlockedLevel();
   // handleUnlockedLevels(evnt);
}
int MainMenu::handleUnlockedLevels(SDL_Event &ev,int unlockedLevel) {
    if (ev.type == SDL_MOUSEBUTTONDOWN)
            {
        if (ev.button.x > 548 && ev.button.x < 790 && ev.button.y>511 && ev.button.y < 579)
        {
            //continue

            switch (unlockedLevel)
            {
            case 1: //no states saved
                //load new game
                return 1;
                //lets dry run
                break;
            case 2: //reached level 2, progress will be changed to 1
                //load level2()
                return 2;
                break;
            case 3: // completed level2, progress value should be changed to 2
                //Game reset,newlevel with toggle val
                
                  toggleLRVal();
                return 3;
                //load level3()
                break;
            }

        }
            else if (ev.button.x > 812 && ev.button.x < 1090 && ev.button.y>511 && ev.button.y < 579)
            {
                //HighScores();
        //std::cout << "Highscores" << std::endl;
                showHighScores();
                //isRunning = false;
            }

            }
}