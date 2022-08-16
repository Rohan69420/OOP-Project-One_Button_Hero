#pragma once
#include "Game.h"

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
		case SDLK_UP: mVelY -= DOT_VEL*3; break;
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
		case SDLK_UP: mVelY += DOT_VEL*3; break;
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

	//collision with first row obstacles
	for (int i = TOTALOBSTACLES - 1; i >= GOODPLATFORMONE; i--) { //need to start form the top not bottom
		if (((mPosY + DOT_HEIGHT) == Obstacle[i].y)&&(mPosX>=Obstacle[i].x && mPosX<=Obstacle[i].x+Obstacle[i].w)) {
			return true;
		}
	}

	return false;
}

void Player::LoadAllObstacles() {
	Obstacle[GOODPLATFORMONE] = { 0,screenH - 20,screenW / 3,20 };
	Obstacle[BADPLATFORMONE] = { screenW / 3,screenH - 20,screenW / 3,20 };
	Obstacle[GOODPLATFORMTWO]= { 2*screenW / 3,screenH - 20,screenW / 3,20 };
	Obstacle[FLOORTWOGOODPLATFORMONE] = { 0,screenH - 300,screenW / 3 - 30,20 };
	Obstacle[FLOORTWOBADPLATFORMONE]= { screenW/3 - 30,screenH - 300,screenW / 3 -30,20 };
	Obstacle[FLOORTWOGOODPLATFORMTWO]= { 2*screenW/3 - 60,screenH - 300,screenW / 3 -40,20 };
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