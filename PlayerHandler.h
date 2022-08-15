#pragma once
#include "Game.h"
#include "GlobalDetails.h"
class Player{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = SPRITEW;
	static const int DOT_HEIGHT = SPRITEH;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 10;
	static const int Gravity = 4;

	//Initializes the variables
	Player();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);					///<<<<<< updated event handler

	//Moves the dot
	void move();	///<<<<<<<<<	-	-	-	-	-	-	-		-

	//Shows the dot on the screen
	void render(SDL_Rect *);
	
	void positiveGravity();

	bool Collision();


private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;					//<<<	<	<	<	<	< -	-	-	-	-

	//The velocity of the dot
	int mVelX, mVelY;

	SDL_RendererFlip FlipVal;

	enum DirectionFacing {
		LEFT,
		RIGHT
	};
	DirectionFacing DF;
};