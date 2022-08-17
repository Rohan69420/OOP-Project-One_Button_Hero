#pragma once
#include "enumerators.h"
#include "Game.h"

class Checkpoint {
private:
	SDL_Rect ckPoint[TOTALCHECKPOINTS],CheckpointSprites[6];

public:
	Checkpoint();
	bool reachedCheckPoint(int PlayerX,int PlayerY);
	void renderCheckPoint(int coinframe);
	void initializeCpSprite();
};