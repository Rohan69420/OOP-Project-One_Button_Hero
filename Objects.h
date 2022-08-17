#pragma once
#include "Game.h"
#include "enumerators.h"

class SolidObjects {
public:
	SDL_Rect Obstacle[TOTALOBSTACLES];

	virtual void LoadAllObstacles() = 0;
	virtual void RenderObstacles(SDL_Renderer *gRenderer,int currentLevel)= 0;
};
