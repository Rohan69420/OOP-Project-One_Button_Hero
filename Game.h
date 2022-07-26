#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "TextureManager.h"
#include "SoundManager.h"
#include "PlayerHandler.h"
#include "GlobalDetails.h"
#include <sstream>
#include "enumerators.h"
#include "Objects.h"
#include "Checkpoints.h"
#include "Boulder.h"
#include "Mainmenu.h"
#include <fstream>


using namespace std;

// BEST IDEA TO NEVER INCLUDE DEFINITIONS IN HEADER FILES!!!!! MAIN CAUSE OF ERRORS

//SDL_Renderer* gRenderer;

//global non class enum for whichmap



enum class RectangleId {
	BACKGROUND_RECT,
	TOTAL_RECTANGLES
};
class Game {
public:
	Game();
	~Game();
	void run();
	void init(const char *, int,int,int, int,Uint32);
	void draw();
	void gameLoop();
	void handleEvents();
	bool loadMedia();
	void LoadAppIcon();
	void LoadingScreen();
	void ClearGlobalRenderer();
	void loadWelcomeText();
	void LevelTransition();
	void RenderAnimatedCharacter();
	void RenderAnimatedCheckpoint(int CheckRectNumber);
	void RenderAnimatedBoulder();
	void GameOver();
	void reset();
	/*void changeToLevel(int level);*/

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* screenSurface;
	GameState gamestate;
	TextureId WhichMap;
	int frame;
	int coinframe;
	int boulderframe;
	bool MapRunning,tickStarted;

	Uint32 startTime;
	//std::stringstream timeInText; //for time displaying on top, THIS NEEDS TO BE GLOBAL JESUS CHRIST

	//loading screen specific
	bool launchedLoadingScreen;
	SDL_Rect ProgressBarOuter, ProgressBarInner;
	SDL_Rect SpriteClips[STATIONARY_ANIMATION_FRAMES];
	int oofCount,boulderNumber;
	int currentLevel,lives,unlockedLevel;
	bool firstCollision;
	bool Started;
	bool onMenu,insideMenu;
	int continuestate;
	std::fstream highscoredata;
	int curHighOof, curHighDodge;
};

// Might not need this \|/
//class surfaceClass{};
//class rectangleClass {
//private:
//	SDL_Rect Rect;
//public:
//	
//};