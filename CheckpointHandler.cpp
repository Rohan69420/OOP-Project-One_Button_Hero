#include "Checkpoints.h"
Checkpoint::Checkpoint() { //might need to friend playerhandler?
	ckPoint[CHECKPOINTONE] = {screenW-67,PLATFORMH_THREE-60,67,60};
	initializeCpSprite();
}
bool Checkpoint::reachedCheckPoint(int PlayerX, int PlayerY) {//need a level dependent parameter
	if (PlayerX >= ckPoint[CHECKPOINTONE].x && PlayerX <= ckPoint[CHECKPOINTONE].x + ckPoint[CHECKPOINTONE].w) {
		return true;
	}
		return false;
}
void Checkpoint::initializeCpSprite() {
	CheckpointSprites[0].x = 0;
	CheckpointSprites[0].y = 0;
	CheckpointSprites[0].w = 67;
	CheckpointSprites[0].h = 60;

	CheckpointSprites[1].x = 67;
	CheckpointSprites[1].y = 0;
	CheckpointSprites[1].w = 67;
	CheckpointSprites[1].h = 60;

	CheckpointSprites[2].x = 134;
	CheckpointSprites[2].y = 0;
	CheckpointSprites[2].w = 67;
	CheckpointSprites[2].h = 60;

	CheckpointSprites[3].x = 201;
	CheckpointSprites[3].y = 0;
	CheckpointSprites[3].w = 67;
	CheckpointSprites[3].h = 60;

	CheckpointSprites[4].x = 268;
	CheckpointSprites[4].y = 0;
	CheckpointSprites[4].w = 67;
	CheckpointSprites[4].h = 60;

	CheckpointSprites[5].x = 335;
	CheckpointSprites[5].y = 0;
	CheckpointSprites[5].w = 67;
	CheckpointSprites[5].h = 60;



}