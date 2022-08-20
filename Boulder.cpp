#pragma once
#include "Boulder.h"


Boulder::Boulder() {
	reset();
	loadBoulderSprites();
}
bool Boulder::BoulderCollision(int PlayerX, int PlayerY) {
	//accuracy can be enhanced, just working prototype for now
	
	if (PlayerY+SPRITEH/2 >= BoulderRect.y && PlayerY+SPRITEH/2 <= BoulderRect.y + BoulderRect.h) { //THIS PAINFUL
		//std::cout << "Y collided" << std::endl;
	//std::cout << BoulderRect.x << "-" << BoulderRect.x+BoulderRect.w << " " << PlayerX << std::endl;
		if ((PlayerX+SPRITEW/2 >= BoulderRect.x) && (PlayerX+SPRITEW/2 <= BoulderRect.x + BoulderRect.w)) {
		//	std::cout << "Total collision" << std::endl;
			return true;
		}
	}
	return false;
}

bool Boulder::DropBoulder() {
	if (BoulderRect.y<=screenH){
		BoulderRect.y += 10;
		return true;
	}
	else {
		SpawnNewBoulder();
		return false; //false is no drop, spawn instead
	}
}
void Boulder :: SpawnNewBoulder() {
	BoulderRect.x = rand() % (screenW-BOULDERWIDTH/2);
	BoulderRect.y = -BOULDERHEIGHT;
}
void Boulder::loadBoulderSprites() {
	BoulderSprites[0].x = 0;
	BoulderSprites[0].y = 0;
	BoulderSprites[0].w = BOULDERWIDTH;
	BoulderSprites[0].h = BOULDERHEIGHT;

	BoulderSprites[1].x = BOULDERWIDTH;
	BoulderSprites[1].y = 0;
	BoulderSprites[1].w = BOULDERWIDTH;
	BoulderSprites[1].h = BOULDERHEIGHT;

	BoulderSprites[2].x = 2*BOULDERWIDTH;
	BoulderSprites[2].y = 0;
	BoulderSprites[2].w = BOULDERWIDTH;
	BoulderSprites[2].h = BOULDERHEIGHT;

	BoulderSprites[3].x = 3*BOULDERWIDTH;
	BoulderSprites[3].y = 0;
	BoulderSprites[3].w = BOULDERWIDTH;
	BoulderSprites[3].h = BOULDERHEIGHT;

	BoulderSprites[4].x = 4*BOULDERWIDTH;
	BoulderSprites[4].y = 0;
	BoulderSprites[4].w = BOULDERWIDTH;
	BoulderSprites[4].h = BOULDERHEIGHT;

	BoulderSprites[5].x = 5*BOULDERWIDTH;
	BoulderSprites[5].y = 0;
	BoulderSprites[5].w = BOULDERWIDTH;
	BoulderSprites[5].h = BOULDERHEIGHT;

	BoulderSprites[6].x = 6*BOULDERWIDTH;
	BoulderSprites[6].y = 0;
	BoulderSprites[6].w = BOULDERWIDTH;
	BoulderSprites[6].h = BOULDERHEIGHT;

	BoulderSprites[7].x = 7*BOULDERWIDTH;
	BoulderSprites[7].y = 0;
	BoulderSprites[7].w = BOULDERWIDTH;
	BoulderSprites[7].h = BOULDERHEIGHT;

}
void Boulder::reset() {
	srand((unsigned)time(NULL));
	BoulderRect.x = rand() % (screenW - BOULDERWIDTH);
	BoulderRect.y = -BOULDERHEIGHT;
	BoulderRect.w = BOULDERWIDTH;
	BoulderRect.h = BOULDERHEIGHT;
}