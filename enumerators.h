#pragma once
enum GameState {
	PLAY,
	EXIT
};

enum TextureId {
	GSPRITESHEETTEXTURE,
	FIRSTMAP,
	GTIMER,
	GOOFCOUNTER,
	WELCOMEOBH,
	PRESSSTART,
	BIGLOGO,
	WALLTEXTURE,
	LAVA,
	CHECKCOIN,
	NEWLEVEL,
	LEVELTWOMAP,
	BOULDER,
	BOULDERCOUNTER,
	LIVES,
	GAMEOVERTEXT,
	MAINMENUANIM,
	LEVELUNLOCKED,
	HIGHSCORE,
	HIGHSCOREVAL,
	TOTALTEXTURES
};
enum ObstacleId {
	GOODPLATFORMONE,
	GOODPLATFORMTWO,

	FLOORTWOGOODPLATFORMONE,
	FLOORTWOGOODPLATFORMTWO,
	FLOORTWOGOODPLATFORMTHREE,

	FLOORTHREEGOODPLATFORMONE,
	FLOORTHREEGOODPLATFORMTWO,

	

	BADPLATFORMONE,
	FLOORTWOBADPLATFORMONE,
	FLOORTWOBADPLATFORMTWO,

	LVLTWOGOODPLATFORMTHREE,
	TOTALOBSTACLES
	
};
enum Checkpoints {
	CHECKPOINTONE,
	TOTALCHECKPOINTS
};