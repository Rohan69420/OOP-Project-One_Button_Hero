#pragma once
#include "Game.h"
#ifndef PLATFORMH_ONE
#define PLATFORMH_ONE screenH-20
#define PLATFORMH_TWO screenH-200
#define PLATFORMH_THREE screenH-400
#define JUMPDIST DOT_VEL*10
#define PLATFORMTHICC 20
#endif

Player::Player() { //need to overload for different levels ig, position wise 
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
void Player::move(int currentLevel) {
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
	if (hoppedOver(currentLevel)) { 
		mPosY -= mVelY;
	}
}
bool Player::hoppedOver(int currentLevel) {
	if (currentLevel == 1) {
		for (int i = TOTALOBSTACLES - 2; i >= GOODPLATFORMONE; i--) { //need to start form the top not bottom
			if ((mPosX >= Obstacle[i].x) && (mPosX <= Obstacle[i].x + Obstacle[i].w)) {
				if ((mPosY < Obstacle[i].y) && (mPosY - mVelY > Obstacle[i].y)) {
					return true;
				}
			}
		}
	}
	if (currentLevel == 2) { //really dont need this in level 2 ig
		if ((mPosX >= Obstacle[LVLTWOGOODPLATFORMTHREE].x) && (mPosX <= Obstacle[LVLTWOGOODPLATFORMTHREE].x + Obstacle[LVLTWOGOODPLATFORMTHREE].w)) {
			if ((mPosY < Obstacle[LVLTWOGOODPLATFORMTHREE].y) && (mPosY - mVelY > Obstacle[LVLTWOGOODPLATFORMTHREE].y)) {
				return true;
			}
		}
	}
	return false;
}

void Player::positiveGravity(int currentLevel) {
	if (!Collision(currentLevel)) {
		mPosY += Gravity;
	}
}
bool Player::Collision(int currentLevel) {
	if ((mPosX < 0) || (mPosY < 0) || (mPosX + DOT_WIDTH) > screenW || (mPosY + DOT_HEIGHT) > screenH) {
		return true;
	}

	//collision with obstacles

	//careful that this only works because increments occur in multiple of 10 and gravity is 2.
	if (currentLevel == 1) {
		for (int i = TOTALOBSTACLES - 1; i >= GOODPLATFORMONE; i--) {
			//need to start form the top not bottom

			// need to move hitbox to center
			if ((mPosY + DOT_HEIGHT) == Obstacle[i].y) {
				if (((mPosX + DOT_WIDTH / 2) >= Obstacle[i].x) && ((mPosX + DOT_WIDTH / 2) <= Obstacle[i].x + Obstacle[i].w)) {
					return true;

				}
			}
		}
	}
	if (currentLevel == 2) {
		//insert level 2 platform collisions here
		if ((mPosY + DOT_HEIGHT) == Obstacle[LVLTWOGOODPLATFORMTHREE].y) {
			if (((mPosX + DOT_WIDTH / 2) >= Obstacle[LVLTWOGOODPLATFORMTHREE].x) && ((mPosX + DOT_WIDTH / 2) <= Obstacle[LVLTWOGOODPLATFORMTHREE].x + Obstacle[LVLTWOGOODPLATFORMTHREE].w)) {
				return true;

			}
		}
	}
	return false;
}
bool Player::BadCollision(int currentLevel) {
	if (currentLevel == 1) {
		for (int i = BADPLATFORMONE;i <= FLOORTWOBADPLATFORMTWO;i++) {
			if ((mPosY + DOT_HEIGHT) == Obstacle[i].y) {
				if (((mPosX + DOT_WIDTH / 2) >= Obstacle[i].x) && ((mPosX + DOT_WIDTH / 2) <= Obstacle[i].x + Obstacle[i].w)) {
					return true;
				}
			}
		}
	}
	return false;
}
void Player::LoadAllObstacles() {
	//first level
	
		//first floor
		Obstacle[GOODPLATFORMONE] = { 0,PLATFORMH_ONE,screenW / 3,PLATFORMTHICC };
		Obstacle[BADPLATFORMONE] = { screenW / 3,PLATFORMH_ONE,screenW / 3,PLATFORMTHICC };
		Obstacle[GOODPLATFORMTWO] = { 2 * screenW / 3,PLATFORMH_ONE,screenW / 3,PLATFORMTHICC };

		//second floor
		Obstacle[FLOORTWOGOODPLATFORMONE] = { 0,PLATFORMH_TWO,screenW / 6,PLATFORMTHICC };
		Obstacle[FLOORTWOBADPLATFORMONE] = { Obstacle[FLOORTWOGOODPLATFORMONE].x + Obstacle[FLOORTWOGOODPLATFORMONE].w,PLATFORMH_TWO,screenW / 8,PLATFORMTHICC };
		Obstacle[FLOORTWOGOODPLATFORMTWO] = { Obstacle[FLOORTWOBADPLATFORMONE].x + Obstacle[FLOORTWOBADPLATFORMONE].w,PLATFORMH_TWO,screenW / 3,PLATFORMTHICC };
		Obstacle[FLOORTWOBADPLATFORMTWO] = { Obstacle[FLOORTWOGOODPLATFORMTWO].x + Obstacle[FLOORTWOGOODPLATFORMTWO].w,PLATFORMH_TWO,Obstacle[FLOORTWOBADPLATFORMONE].w,PLATFORMTHICC };
		Obstacle[FLOORTWOGOODPLATFORMTHREE] = { Obstacle[FLOORTWOBADPLATFORMTWO].x + Obstacle[FLOORTWOBADPLATFORMTWO].w,PLATFORMH_TWO,screenW / 6,PLATFORMTHICC };

		//thirdfloor
		Obstacle[FLOORTHREEGOODPLATFORMONE] = { Obstacle[FLOORTWOBADPLATFORMONE].x + Obstacle[FLOORTWOBADPLATFORMONE].w,PLATFORMH_THREE,screenW / 3,PLATFORMTHICC };
		Obstacle[FLOORTHREEGOODPLATFORMTWO] = { Obstacle[FLOORTWOBADPLATFORMTWO].x + Obstacle[FLOORTWOBADPLATFORMTWO].w,PLATFORMH_THREE,screenW / 4,PLATFORMTHICC };
	

	//level two
	
		//only one floor
		Obstacle[LVLTWOGOODPLATFORMTHREE] = { 0,screenH - 200,screenW,50 };
	
}
void Player::ResetPos(int currentLevel) {
	//first level reset
	if (currentLevel == 1) {
		mPosX = 0;
		mPosY = screenH - 20 - DOT_HEIGHT;
	}
	if (currentLevel == 2) {
		mPosX = 0;
		mPosY = Obstacle[LVLTWOGOODPLATFORMTHREE].y-DOT_HEIGHT;
	}
}
int Player::getPlayerXPos() {
	return mPosX;
}
int Player::getPlayerYPos() {
	return mPosY;
}