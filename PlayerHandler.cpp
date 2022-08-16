#pragma once
#include "Game.h"

#define PLATFORMH_ONE screenH-20
#define PLATFORMH_TWO screenH-200
#define JUMPDIST DOT_VEL*10

Player::Player() {
	mPosX = 0;
	mPosY = screenH-20-DOT_HEIGHT;
	mVelX = 0;
	mVelY = 0;

	//facing directions
	FlipVal = SDL_FLIP_NONE;
	DF = RIGHT; //persisting issue that it gets reset to this variable once scope ends but we dont need to tackle this issue in our original plan

	LoadAllObstacles();
	
	//jump counter capped to 3 i.e, three jumps maximum allowed 
	jumpCounter = 3;
}
void Player::handleEvent(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)		///<<<<<<<<	-	-	- need key repeat because only one press is enuff
	{									////////<<<< meaning that even if you are holding key it will still by 10fps vel
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= JUMPDIST; break;
		//case SDLK_DOWN: mVelY += DOT_VEL; break;
		case SDLK_LEFT: mVelX -= DOT_VEL;
			if (DF == RIGHT) {
				DF = LEFT;
				FlipVal = SDL_FLIP_HORIZONTAL;
			}
			else {
				FlipVal = SDL_FLIP_NONE;
			}
			break;

		case SDLK_RIGHT: mVelX += DOT_VEL;
			if (DF == LEFT) {
				DF = RIGHT;
				FlipVal = SDL_FLIP_HORIZONTAL;
			}
			else {
				FlipVal = SDL_FLIP_NONE;
			}
			break;
		}
	}
	//Release
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)		//<<<<<<<<< reverse velocity?
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += JUMPDIST; break;
		//case SDLK_DOWN: mVelY -= DOT_VEL; break;
		case SDLK_LEFT: mVelX += DOT_VEL; break;
		case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}
	}
}
void Player::move() {
	//Move the dot left or right
	mPosX += mVelX;						///<<<<<<<<<< move as much as the velocity

	//If the dot went too far to the left or right
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > screenW))		//<<move back nice
	{
		//Move back
		mPosX -= mVelX;
	}

	//Move the dot up or down
	mPosY += mVelY;

	//If the dot went too far up or down
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT > screenH))
	{
		//Move back
		mPosY -= mVelY;
	}

	//PREVENT JUMPING WHERE NOT ALLOWED
	if (hoppedOver()) { 
		mPosY -= mVelY;
	}
}
bool Player::hoppedOver() {
	for (int i = TOTALOBSTACLES - 1; i >= GOODPLATFORMONE; i--) { //need to start form the top not bottom
		if ((mPosX >= Obstacle[i].x) && (mPosX <= Obstacle[i].x + Obstacle[i].w)) {
			if ((mPosY< Obstacle[i].y) && (mPosY - mVelY > Obstacle[i].y)) {
				return true;
			}
		}
	}
	return false;
}

void Player::positiveGravity() {
	if (!Collision()) {
		mPosY += Gravity;
	}
}
bool Player::Collision() {
	if ((mPosX < 0) || (mPosY < 0) || (mPosX + DOT_WIDTH) > screenW || (mPosY + DOT_HEIGHT) > screenH) {
		return true;
	}

	//collision with obstacles

	//careful that this only works because increments occur in multiple of 10 and gravity is 2.
	for (int i = TOTALOBSTACLES - 1; i >= GOODPLATFORMONE; i--) { //need to start form the top not bottom
		if (((mPosY + DOT_HEIGHT) == Obstacle[i].y)&&(mPosX>=Obstacle[i].x && mPosX<=Obstacle[i].x+Obstacle[i].w)) {
			return true;
		}
	}

	return false;
}

void Player::LoadAllObstacles() {
	//first floor
	Obstacle[GOODPLATFORMONE] = { 0,PLATFORMH_ONE,screenW / 3,20 };
	Obstacle[BADPLATFORMONE] = { screenW / 3,PLATFORMH_ONE,screenW / 3,20 };
	Obstacle[GOODPLATFORMTWO]= { 2*screenW / 3,PLATFORMH_ONE,screenW / 3,20 };

	//second floor
	Obstacle[FLOORTWOGOODPLATFORMONE] = { 0,PLATFORMH_TWO,screenW / 3 - 30,20 };
	Obstacle[FLOORTWOBADPLATFORMONE]= { screenW/3 - 30,PLATFORMH_TWO,screenW / 3 -30,20 };
	Obstacle[FLOORTWOGOODPLATFORMTWO]= { 2*screenW/3 - 60,PLATFORMH_TWO,screenW / 3 -40,20 };
}
void Player::RenderObstacles(SDL_Renderer* gRenderer) {
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); //green
	for (int i = GOODPLATFORMONE;i <= FLOORTWOGOODPLATFORMTWO;i++) {
		SDL_RenderFillRect(gRenderer, &Obstacle[i]);
	}
	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); //red
	for (int i = BADPLATFORMONE;i <= FLOORTWOBADPLATFORMONE;i++) {
		SDL_RenderFillRect(gRenderer, &Obstacle[i]);
	}
}