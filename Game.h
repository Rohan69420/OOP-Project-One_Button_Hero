#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "TextureManager.h"
#include "SoundManager.h"
#include <sstream>

#define STATIONARY_ANIMATION_FRAMES 8

using namespace std;

// BEST IDEA TO NEVER INCLUDE DEFINITIONS IN HEADER FILES!!!!! MAIN CAUSE OF ERRORS

//SDL_Renderer* gRenderer;

//global non class enum for whichmap
enum MapID {
	WELCOMESCREEN,
	TOTALMAPS
};

enum class GameState {
	PLAY,
	EXIT
};
enum class TextureId {
	BACKGROUND_TEXTURE,
	TOTAL_TEXTURES
};
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
    void loadGlobalText();
	void LoadAppIcon();
	void LoadingScreen();
	void ClearGlobalRenderer();
	void loadWelcomeText();
private:
	int screenW, screenH;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* screenSurface;
	GameState gamestate;
	MapID WhichMap;
	int frame;
	bool MapRunning,tickStarted;

	Uint32 startTime;
	//std::stringstream timeInText; //for time displaying on top, THIS NEEDS TO BE GLOBAL JESUS CHRIST

	//loading screen specific
	bool launchedLoadingScreen;
	SDL_Rect ProgressBarOuter, ProgressBarInner;
	SDL_Rect SpriteClips[STATIONARY_ANIMATION_FRAMES];
};

class surfaceClass{};
class rectangleClass {
private:
	SDL_Rect Rect;
public:
	
};