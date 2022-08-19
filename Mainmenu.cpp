#include "Mainmenu.h"
MainMenu::MainMenu(){
    rectDes.x = rectDes.y = 0;
    rectDes.w = 1140;
    rectDes.h = 600;
    rectSrc.x = rectSrc.y = 0;
    rectSrc.w = 1140;
    rectSrc.h = 600;
    exit = false;
    state = 0;
	loadMenuSprites();
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
void MainMenu::HandleMenuEvent(SDL_Event& e) {
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
         if (rectSrc.x == 1140 && rectSrc.y == 0) {
             state = 1;
         }
     }
     RenderMenu();
}
int MainMenu::MenuAction() {
    return state;
}