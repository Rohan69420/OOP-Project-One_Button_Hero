#pragma once
#include "Game.h"
#include "GlobalDetails.h"
#include "Objects.h"
#include "TextureManager.h"

class Player : public SolidObjects{ //inheritance
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = SPRITEW;
	static const int DOT_HEIGHT = SPRITEH;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 10;
	static const int Gravity = 2;

	//Initializes the variables
	Player();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);					///<<<<<< updated event handler

	//Moves the dot
	void move(int currentLevel);	///<<<<<<<<<	-	-	-	-	-	-	-		-

	//Shows the dot on the screen
	void render(SDL_Renderer *gRenderer,SDL_Rect *);
	
	void positiveGravity(int currentLevel);

	bool Collision(int currentLevel);

	void LoadAllObstacles();

	void RenderObstacles(SDL_Renderer *gRenderer,int currentLevel);

	bool hoppedOver(int currentLevel);
	
	bool BadCollision(int currentLevel);

	void ResetPos(int currentLevel);

	int getPlayerXPos();
	
	int getPlayerYPos();

	void reset();
	void switchLRlock();

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;					//<<<	<	<	<	<	< -	-	-	-	-

	//The velocity of the dot
	int mVelX, mVelY,AutoVelX,AutoVelYLvl2;

	SDL_RendererFlip FlipVal;

	enum DirectionFacing {
		LEFT,
		RIGHT
	};
	DirectionFacing DF;

	int jumpCounter;

	bool LRlock, left,alreadyTriggered;

};