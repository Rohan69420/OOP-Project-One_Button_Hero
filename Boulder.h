#pragma once
#include "enumerators.h"
#include "Game.h"
#include <cstdlib>

class Boulder {
private:
	SDL_Rect BoulderRect,BoulderSprites[8];

public:
	Boulder();
	bool BoulderCollision(int PlayerX, int PlayerY);
	void renderBoulder(int boulderframe);
	void SpawnNewBoulder();
	void DropBoulder();
	void loadBoulderSprites();
};