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
	reset();
	LoadAllObstacles();
	
	
}
void Player::reset() {
	mPosX = 0;
	mPosY = screenH - 20 - CHAR_HEIGHT;
	mVelX = 0;
	mVelY = 0;
	LRlock = true;
	left = true;
	AutoVelX = 4;
	AutoVelYLvl2 = 10;
	alreadyTriggered = false;

	//facing directions
	FlipVal = SDL_FLIP_NONE;
	DF = RIGHT; //persisting issue that it gets reset to this variable once scope ends but we dont need to tackle this issue in our original plan

	//LoadAllObstacles();

	//jump counter capped to 3 i.e, three jumps maximum allowed 
	jumpCounter = 0;
}
void Player::handleEvent(SDL_Event& e) {
	
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)		///<<<<<<<<	-	-	- need key repeat because only one press is enuff
		{									////////<<<< meaning that even if you are holding key it will still by 10fps vel
			switch (e.key.keysym.sym)
			{
			case SDLK_UP: 
				if (jumpCounter <= 4) { //do nothing if jump counter exceeds
					mVelY -= JUMPDIST;
					++jumpCounter;	//increment jump counter
				}
				if (LRlock) {
					if (left) {
						left = false;

					}
					else {
						left = true;

					}
					alreadyTriggered = false; //reset
				}
				break;
				//case SDLK_DOWN: mVelY += DOT_VEL; break;

				if (!LRlock) {
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
		}
		//Release
		else if (e.type == SDL_KEYUP && e.key.repeat == 0)		//<<<<<<<<< reverse velocity?
		{
			switch (e.key.keysym.sym)
			{
			//case SDLK_UP: mVelY += JUMPDIST;  break;
				//case SDLK_DOWN: mVelY -= DOT_VEL; break;
				if (!LRlock) {
			case SDLK_LEFT: mVelX += DOT_VEL; break;
			case SDLK_RIGHT: mVelX -= DOT_VEL; break;
				}
			}
		}
	
}
void Player::move(int currentLevel) {
	
	if (currentLevel == 1) {
		//Move the dot up or down
		mPosY += mVelY;
		

		//If the dot went too far up or down
		if ((mPosY < 0) || (mPosY + CHAR_HEIGHT > screenH))
		{
			//Move back
			mPosY -= mVelY;
		}

		if (!LRlock) {
			//Move the dot left or right
			mPosX += mVelX;						///<<<<<<<<<< move as much as the velocity

			//If the dot went too far to the left or right
			if ((mPosX < 0) || (mPosX + CHAR_WIDTH > screenW))		//<<move back nice
			{
				//Move back
				mPosX -= mVelX;
			}

		}
		else {
			if (mPosX < 0) {
				AutoVelX = -AutoVelX;
				FlipVal = SDL_FLIP_NONE;
			}
			else if (mPosX + CHAR_WIDTH > screenW) {
				FlipVal = SDL_FLIP_HORIZONTAL; //flip sprite
				AutoVelX = -AutoVelX;
			}

			mPosX += AutoVelX;
		}

		//PREVENT JUMPING WHERE NOT ALLOWED
		if (hoppedOver(currentLevel)) {
			mPosY -= mVelY;
		}

		//reset after checking if hopped over
		mVelY = 0; //reset after jump
	}
	else if (currentLevel == 2) {
		/*std::cout << "Level two motion trigger .Left: " <<std::boolalpha<<left<< std::endl;*/
		if (!LRlock) {
		
			//Move the dot up or down
			mPosY += mVelY;

			//If the dot went too far up or down
			if ((mPosY < 0) || (mPosY + CHAR_HEIGHT > screenH))
			{
				//Move back
				mPosY -= mVelY;
			}
			//Move the dot left or right
			mPosX += mVelX;						///<<<<<<<<<< move as much as the velocity

			//If the dot went too far to the left or right
			if ((mPosX < 0) || (mPosX + CHAR_WIDTH > screenW))		//<<move back nice
			{
				//Move back
				mPosX -= mVelX;
			}

			
		}
		else {
			/*std::cout << "inside !lrlock" << std::endl;*/
			if (left) {
				mPosX -= AutoVelYLvl2;
				if (!alreadyTriggered) {
					FlipVal = SDL_FLIP_HORIZONTAL;
					alreadyTriggered = true;
				}
				/*std::cout << "AutoVel Triggered" << std::endl;*/
			}
			else {
				if (!alreadyTriggered) {
					FlipVal = SDL_FLIP_NONE;
					alreadyTriggered = true;
				}
				mPosX += AutoVelYLvl2;
			}



			if (mPosX < 0) 		//<<move back nice
			{
				//Move back
				mPosX += AutoVelYLvl2;
			}
			else if (mPosX + CHAR_WIDTH > screenW) {
				mPosX -= AutoVelYLvl2;
			}
		}
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
void Player::move() {

	
		//Move the dot up or down
		mPosY += mVelY;


		//If the dot went too far up or down
		if ((mPosY < 0) || (mPosY + CHAR_HEIGHT > screenH))
		{
			//Move back
			mPosY -= mVelY;
		}

		if (!LRlock) {
			//Move the dot left or right
			mPosX += mVelX;						///<<<<<<<<<< move as much as the velocity

			//If the dot went too far to the left or right
			if ((mPosX < 0) || (mPosX + CHAR_WIDTH > screenW))		//<<move back nice
			{
				//Move back
				mPosX -= mVelX;
			}

		}
		else {
			if (mPosX < 0) {
				AutoVelX = -AutoVelX;
				FlipVal = SDL_FLIP_NONE;
			}
			else if (mPosX + CHAR_WIDTH > screenW) {
				FlipVal = SDL_FLIP_HORIZONTAL; //flip sprite
				AutoVelX = -AutoVelX;
			}

			mPosX += AutoVelX;
		

		//PREVENT JUMPING WHERE NOT ALLOWED
		if (hoppedOver(1)) {
			mPosY -= mVelY;
		}

		//reset after checking if hopped over
		mVelY = 0; //reset after jump
	}
	
}
void Player::positiveGravity(int currentLevel) {
	if (!Collision(currentLevel)) {
		mPosY += Gravity;
	}
}
bool Player::Collision(int currentLevel) {
	if ((mPosX < 0) || (mPosY < 0) || (mPosX + CHAR_WIDTH) > screenW || (mPosY + CHAR_HEIGHT) > screenH) {
		return true;
	}

	//collision with obstacles

	//careful that this only works because increments occur in multiple of 10 and gravity is 2.
	if (currentLevel == 1) {
		for (int i = TOTALOBSTACLES - 1; i >= GOODPLATFORMONE; i--) {
			//need to start form the top not bottom

			// need to move hitbox to center
			if ((mPosY + CHAR_HEIGHT) == Obstacle[i].y) {
				if (((mPosX + CHAR_WIDTH / 2) >= Obstacle[i].x) && ((mPosX + CHAR_WIDTH / 2) <= Obstacle[i].x + Obstacle[i].w)) {
					jumpCounter = 0;
					return true;

				}
			}
		}
	}
	if (currentLevel == 2) {
		//insert level 2 platform collisions here
		if ((mPosY + CHAR_HEIGHT) == Obstacle[LVLTWOGOODPLATFORMTHREE].y) {
			if (((mPosX + CHAR_WIDTH / 2) >= Obstacle[LVLTWOGOODPLATFORMTHREE].x) && ((mPosX + CHAR_WIDTH / 2) <= Obstacle[LVLTWOGOODPLATFORMTHREE].x + Obstacle[LVLTWOGOODPLATFORMTHREE].w)) {
				return true;

			}
		}
	}
	return false;
}
bool Player::BadCollision(int currentLevel) {
	if (currentLevel == 1) {
		for (int i = BADPLATFORMONE;i <= FLOORTWOBADPLATFORMTWO;i++) {
			if ((mPosY + CHAR_HEIGHT) == Obstacle[i].y) {
				if (((mPosX + CHAR_WIDTH / 2) >= Obstacle[i].x) && ((mPosX + CHAR_WIDTH / 2) <= Obstacle[i].x + Obstacle[i].w)) {
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
		mPosY = screenH - 20 - CHAR_HEIGHT;
	}
	if (currentLevel == 2) {
		mPosX = 0;
		mPosY = Obstacle[LVLTWOGOODPLATFORMTHREE].y-CHAR_HEIGHT;
	}
}
int Player::getPlayerXPos() {
	return mPosX;
}
int Player::getPlayerYPos() {
	return mPosY;
}
void Player::switchLRlock() {
	if (LRlock) {
		LRlock = !LRlock;
	}
}