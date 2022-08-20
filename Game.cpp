#include "Game.h" 
#include <chrono>
#include <thread>
#include <iomanip>
#include "enumerators.h"

#ifndef SPRITEW
#define SPRITEW 60
#define SPRITEH 59
#define TIMERWIDTH 360
#endif

//removed polluting namespace

//global chrono delay
std::chrono::milliseconds timespan(500); 


//temporary global function
TTF_Font* gfont;
//TTF_Font* globalFont; //Global font declaration, works here?
SDL_Renderer* gRenderer;

textureClass AllTexture;

gameSounds MegaSoundObj;

Mix_Music* gMusic;
std::stringstream timeInText,oofInText,bouldersDodged,liveCounter; ///for the timer text
SDL_Color BlackColor = { 0,0,0 },WhiteColor={0xFF,0xFF,0xFF};

Player P1;
Checkpoint CP1;
Boulder BOU;
MainMenu MM;

Game::Game() { //constructor
	gamestate = PLAY;
	gMusic = NULL;
	//gfont = NULL;
	launchedLoadingScreen = false;
	unlockedLevel = 1;
	reset();
	//WhichMap = FIRSTMAP;
	////STATIONARY_ANIMATION_FRAMES = 8;
	//frame = 0;
	//coinframe = 0;
	//boulderframe = 0;
	////SpriteClips = new SDL_Rect[STATIONARY_ANIMATION_FRAMES]; //LOOKS LIKE IT WORKED

	//startTime = 0;
	//tickStarted = false;

	//
	//oofCount = 0;
	//currentLevel = 1;
	//boulderNumber = 0;
	//lives = 3;
	//firstCollision = true;
	//MapRunning = true;
	//Started = false;
	//onMenu = true;
	//insideMenu = false;
}
void Game::reset() {
	WhichMap = FIRSTMAP;
	//STATIONARY_ANIMATION_FRAMES = 8;
	frame = 0;
	coinframe = 0;
	boulderframe = 0;
	//SpriteClips = new SDL_Rect[STATIONARY_ANIMATION_FRAMES]; //LOOKS LIKE IT WORKED

	startTime = 0;
	tickStarted = false;
	oofCount = 0;
	currentLevel = 1;
	boulderNumber = 0;
	lives = 3;
	firstCollision = true;
	MapRunning = true;
	Started = false;
	onMenu = true;
	insideMenu = false;
	continuestate = 0;
}
Game::~Game() { //destructor
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Game::run() {
	init("One Button Hero",50,50,screenW,screenH,SDL_WINDOW_SHOWN); //CAN DO FULLSCREEN MODE HERE
}
void Game::init(const char *title, int x, int y, int w, int h, Uint32 flags) {
	//initialize everything
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL. Error code: "<<SDL_GetError() << std::endl;
	}
	else {
		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == -1) {
			std::cout << "Unable to initialize jpg and png" << std::endl;
		}
		//initializing SDL ttf
		if (TTF_Init() == -1)
		{
			std::cout << "Unable to initialize SDL TTF. Error code:" <<TTF_GetError()<<std::endl;
		}
		
		window=SDL_CreateWindow(title, x, y, w, h,flags);
		if (window == NULL) {
			std::cout << "Unable to create window! Error code:" << SDL_GetError() << std::endl;
		}
		else
		{
			//load window
			screenSurface = SDL_GetWindowSurface(window);
			//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, reinterpret_cast<char*>(0));
			SDL_RenderSetLogicalSize(gRenderer, screenW, screenH);
			loadMedia();
			LoadAppIcon();
			
			//LoadAppIcon();
			std::this_thread::sleep_for(timespan);
			LoadingScreen();
			//std::chrono::milliseconds timespan(500); // or whatever

			std::this_thread::sleep_for(timespan);
			LoadingScreen();
			ClearGlobalRenderer();
		}
		
		//>>>>>>>>>cclear the renderer before this, or move the welcome text to another corner<<<<<<<<<<<<
		loadWelcomeText();
		MegaSoundObj.playGlobalMusic(); //should be bool but ok
		gameLoop();

	}
}
void Game::gameLoop() {
	//draw();
	
		while (gamestate != EXIT) {
			handleEvents();
			if (unlockedLevel <= 3) { //if not freeplay need an extra parameter
				P1.move(currentLevel);
			}
			//P1.move(); //what if move is shifted inside handle events?
			//onmenu locked
			if (!onMenu) {
				if (MapRunning && Started) {
					if (!tickStarted) {
						startTime = SDL_GetTicks();
						tickStarted = true;
					}
					draw();
				}
			}
		}
	
}
void Game::handleEvents() {
	SDL_Event evnt;
	if (MapRunning == true) {
		P1.positiveGravity(currentLevel);
	}

	while (SDL_PollEvent(&evnt)) {
		//global exit handler, cause it should run regardless of game condition
		switch (evnt.type) {
		case SDL_QUIT:
			gamestate = EXIT;
			break;
		case SDL_KEYDOWN:
			if (evnt.key.keysym.sym == SDLK_0) { //need the pause option for my ears
				if (Mix_PlayingMusic() == 0)
				{
					//Play the music
					Mix_PlayMusic(gMusic, -1);
				}
				//If music is being played
				else
				{
					//If the music is paused
					if (Mix_PausedMusic() == 1)
					{
						//Resume the music
						Mix_ResumeMusic();
					}
					//If the music is playing
					else
					{
						//Pause the music
						Mix_PauseMusic();
					}
				}
			}
			break;
		}

		if (!onMenu) {
			switch (evnt.type) {
			case SDL_KEYDOWN:
				
					if (MapRunning == true) {
						P1.handleEvent(evnt);
						P1.move(currentLevel);
						draw();
					}
				break;

			case SDL_KEYUP:
				if (MapRunning == true) {
					P1.handleEvent(evnt);
					P1.move(currentLevel);
					draw();
				}
				break;

				//removed the resizable part altogether

			}
		}
		if (onMenu) {
			if (evnt.type == SDL_KEYDOWN ) {
				insideMenu = true;
			}
			if (insideMenu) {
				continuestate=MM.HandleMenuEvent(evnt,unlockedLevel);
			}
			switch(MM.MenuAction()){
			case 1:
				onMenu = false; //these setting to go to levels
				insideMenu = false;
				unlockedLevel = 1;
				draw();
				break;
			case 2:
				//progressbar
				MM.unlockedLevels(unlockedLevel,evnt);
				if (continuestate == 1) {
					onMenu = false; //basically copied from first case
					insideMenu = false;
					draw();
				}
				else if (continuestate == 2) {
					onMenu = false; //basically copied from first case
					insideMenu = false;
					currentLevel = 2; //pray this workey;
					P1.ResetPos(currentLevel);
					draw();
				}
				else if(continuestate==3){
					onMenu = false; //basically copied from first case
					insideMenu = false;
					currentLevel = 2; //pray this workey;
					P1.ResetPos(currentLevel);
					draw();
					//plus unlock the left right keys boolean here
				}
				break;
			case 3:
				gamestate = EXIT;
				break;
			}
		}
	}
}
void Game::draw() {
	MapRunning = true;
	Started = true;
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	if (currentLevel == 1) { //level one condition
		
		//check collision before drawing
		if (P1.BadCollision(currentLevel)) {
			P1.ResetPos(currentLevel);
			oofCount++;
		}
	

		//render background map 1
		AllTexture.render(gRenderer, WhichMap, 0, 0);


		//render obstacles
		P1.RenderObstacles(gRenderer,currentLevel);

		RenderAnimatedCharacter(); //character, player class dependent, no need any parameter passing
		RenderAnimatedCheckpoint(CHECKPOINTONE); //coin

		oofInText.str(""); // TURNS OUT THIS IS FOR CLEARING THE PAST TEXT i.e, reset
		oofInText << "Oof counter: " << oofCount << setw(5);

		AllTexture.loadFromRenderedText(gRenderer, GOOFCOUNTER, oofInText.str().c_str(), BlackColor, gfont);
		AllTexture.render(gRenderer, GOOFCOUNTER, 0, 0);
		
		//check if checkpoint reached
		if (CP1.reachedCheckPoint(P1.getPlayerXPos(), P1.getPlayerYPos())) {

			//call transition
			LevelTransition();
			std::cout << "Transition ready!" << std::endl;

		}
		if (oofCount == 10) {
			GameOver();
		}
	}

	if (currentLevel == 2) { //level two condition
		

		//test collision and increment
		if (!BOU.DropBoulder()) {
			++boulderNumber;
			firstCollision = true;
		}
		
		if (BOU.BoulderCollision(P1.getPlayerXPos(), P1.getPlayerYPos()) && firstCollision) {
			--lives;
			firstCollision = false;
		}

		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);
		AllTexture.render(gRenderer, WhichMap, 0, 0);
		RenderAnimatedCharacter();
		RenderAnimatedBoulder();

		P1.RenderObstacles(gRenderer, currentLevel);

		//counting dodges
		bouldersDodged.str("");
		bouldersDodged << "Dodges: " << boulderNumber << setw(5);

		AllTexture.loadFromRenderedText(gRenderer, BOULDERCOUNTER, bouldersDodged.str().c_str(), BlackColor, gfont);
		AllTexture.render(gRenderer, BOULDERCOUNTER, 0, 0);

		//counting lives
		liveCounter.str("");
		liveCounter << "Lives: " << lives << setw(5);

		AllTexture.loadFromRenderedText(gRenderer, LIVES, liveCounter.str().c_str(), WhiteColor, gfont);
		AllTexture.render(gRenderer, LIVES, screenW-AllTexture.getWidth(LIVES), screenH - AllTexture.getHeight(LIVES) - 20);
		
		//render collision frame and pause for two seconds before deeming game over
		if (lives == 0) {
			SDL_Delay(2000);
			MapRunning = false; 
			GameOver();
		}
		if (boulderNumber == 36) {
			unlockedLevel = 3;
		}
	}

	timeInText.str(""); // TURNS OUT THIS IS FOR CLEARING THE PAST TEXT i.e, reset
	timeInText << "Time passed: " << (SDL_GetTicks() - startTime) / 1000 << setw(5) << " s";

	AllTexture.loadFromRenderedText(gRenderer, GTIMER, timeInText.str().c_str(), BlackColor, gfont);
	AllTexture.render(gRenderer, GTIMER, screenW - TIMERWIDTH, 0);

	
	//Update screen
	SDL_RenderPresent(gRenderer);
}
void Game::RenderAnimatedCheckpoint(int CheckRectNumber) {
	//rendering coin

	CP1.renderCheckPoint(coinframe,CheckRectNumber);
	//next frame
	++coinframe;

	//cycle animation
	if (coinframe / 6 >= COIN_ANIMATION_FRAMES) {
		coinframe = 0;
	}

}

void Game::RenderAnimatedCharacter() {
	SDL_Rect* currentClip = &SpriteClips[frame / 8]; //adjust framerate
	P1.render(gRenderer, currentClip); //RenderCharacter here
	//Go to next frame
	++frame;
	//Cycle animation
	if (frame / 8 >= STATIONARY_ANIMATION_FRAMES)
	{
		frame = 0;
	}
}

void Game::loadWelcomeText() {
	
	SDL_RenderClear(gRenderer);//clear
	//sorted that we dont need to resize and can use point size instead
	AllTexture.render(gRenderer,WELCOMEOBH,(screenW-AllTexture.getWidth(WELCOMEOBH))/2, (screenH-AllTexture.getHeight(WELCOMEOBH)) / 2);  //default argument enabled
	//positioned such as the welcome text is rendered below the press to start
	AllTexture.render(gRenderer,PRESSSTART,(screenW-AllTexture.getWidth(PRESSSTART)) / 2, (screenH-AllTexture.getHeight(PRESSSTART)) / 2 + AllTexture.getHeight(PRESSSTART) + 5);
	//giving some clearance
	//SDL_RenderCopy(gRenderer, mTexture, NULL, &WelcomeText); //<here it is being rendered
	SDL_RenderPresent(gRenderer);
	

}
void Game::LoadAppIcon() {
	SDL_SetWindowIcon(window, IMG_Load("images/ICON.png"));
	std::cout << "Load app icon launched" << std::endl;
	LoadingScreen();
	//loading screen
}
void Game::LoadingScreen() {
	if (!launchedLoadingScreen) {
		//temporary clipping solution
		SDL_Rect clipper;
		clipper = { screenSurface->w / 3,screenSurface->h / 6,screenSurface->w / 3,screenSurface->w / 3 };
		AllTexture.renderResized(gRenderer,BIGLOGO,&clipper); //default args

		ProgressBarOuter = { screenSurface->w / 6,screenSurface->h / 6 + clipper.h + 20 ,static_cast<int>(screenSurface->w / 1.5),10 };
		//y position set such that it is always below the big logo, inner rect y-pos is same as outer y-pos
		
		ProgressBarInner = { ProgressBarOuter.x + 2,ProgressBarOuter.y ,0 / 3,ProgressBarOuter.h - 2 };
		launchedLoadingScreen = true;
		//first logo launch
		
	}
	//position the SDL_Rect, do it the savage way and clean up late

	//should use sdl renderfill rect orrrrrrrr
	//i mean yeah renderer would be better than writing on the surface
	if (ProgressBarInner.w <= ProgressBarOuter.w) {
		ProgressBarInner.w += ProgressBarOuter.w / 3;
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &ProgressBarOuter);
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderFillRect(gRenderer, &ProgressBarInner);
		//do we need to render copy with render fill? turns out the answer is a no
		SDL_RenderPresent(gRenderer);
	}
}


bool Game::loadMedia() {

	//fonts 
	gfont = TTF_OpenFont("ka1.ttf", 72); //ptsize is size of font 

	if (gfont == NULL) {
		std::cout << "Failed to load the ttf file. Error code" << TTF_GetError() << std::endl;
	}

	SDL_Color textColor = { 255, 255, 255 };

	if (!AllTexture.loadFromRenderedText(gRenderer,WELCOMEOBH, "Welcome to One Button Hero", textColor, gfont)) {
		std::cout << "Failed to render texture." << std::endl;
	}
	gfont = TTF_OpenFont("ka1.ttf", 48);
	if (!AllTexture.loadFromRenderedText(gRenderer,PRESSSTART, "Press any button to start.", textColor, gfont)) {
		std::cout << "Failed to render texture." << std::endl;
	}

	//timer
	


	//SOUND AREA


	if (!MegaSoundObj.initSounds()) {
		std::cout << "Unable to initialize the mixer!!!!" << std::endl;
	}
	if (!MegaSoundObj.loadAllSounds()) {
		std::cout << "Unable to load all sounds from file!!!" << std::endl;
	}

	//images

	if (!AllTexture.loadFromFile(gRenderer, BIGLOGO, "images/LOGO.png")) {
		std::cout << "Failed to load the biglogo image!" << std::endl;
	}

	if (!AllTexture.loadFromFile(gRenderer,GSPRITESHEETTEXTURE,"nicepng60pxw.png"))
	{
		std::cout<<"Failed to load walking animation texture!\n"<<std::endl;
		//success = false;
	}
	if (!AllTexture.loadFromFile(gRenderer, WALLTEXTURE, "WallAtMyExpense.png")) {
		std::cout << "Failed to load the wall texture!" << std::endl;
	}
	if (!AllTexture.loadFromFile(gRenderer, LAVA, "Lava.png")) {
		std::cout << "Failed to load the lava texture!" << std::endl;
	}
	if (!AllTexture.loadFromFile(gRenderer, CHECKCOIN, "checkcoin1.png")) {
		std::cout << "Failed to load the coin sprite" << std::endl;
	}
	if (!AllTexture.loadFromFile(gRenderer, LEVELTWOMAP, "lvl2map.png")) {
		std::cout << "Failed to load level two map" << std::endl;
	}
	if (!AllTexture.loadFromFile(gRenderer, BOULDER, "boulder.png")) {
		std::cout << "Failed to load boulder spritesheet" << std::endl;
	}
	if (!AllTexture.loadFromFile(gRenderer, MAINMENUANIM, "Menu.png")) {
		std::cout << "Failed to load the main menu animated texture" << std::endl;
	}
	if (!AllTexture.loadFromFile(gRenderer, LEVELUNLOCKED, "progress.png")) {
		std::cout<<"Failed to load the level unlockedscreen"<<std::endl;
	}
	if (!AllTexture.loadFromFile(gRenderer, FIRSTMAP, "Map1.png")) {
		std::cout << "Failed to load the first background map image" << std::endl;
	}
	else
	{
		//Set sprite clips
		SpriteClips[0].x = 0;
		SpriteClips[0].y = 119;
		SpriteClips[0].w = SPRITEW;
		SpriteClips[0].h = SPRITEH;

		SpriteClips[1].x = 60;
		SpriteClips[1].y = 119;
		SpriteClips[1].w = SPRITEW;
		SpriteClips[1].h = SPRITEH;

		SpriteClips[2].x = 60 * 2;
		SpriteClips[2].y = 119;
		SpriteClips[2].w = SPRITEW;
		SpriteClips[2].h = SPRITEH;

		SpriteClips[3].x = 60 * 3;
		SpriteClips[3].y = 119;
		SpriteClips[3].w = SPRITEW;
		SpriteClips[3].h = SPRITEH;

		SpriteClips[4].x = 60 * 4;
		SpriteClips[4].y = 119;
		SpriteClips[4].w = SPRITEW;
		SpriteClips[4].h = SPRITEH;

		SpriteClips[5].x = 60 * 5;
		SpriteClips[5].y = 119;
		SpriteClips[5].w = SPRITEW;
		SpriteClips[5].h = SPRITEH;

		SpriteClips[6].x = 60 * 6 + 2;
		SpriteClips[6].y = 119;
		SpriteClips[6].w = SPRITEW;
		SpriteClips[6].h = SPRITEH;

		SpriteClips[7].x = 60 * 7 + 4;
		SpriteClips[7].y = 119;
		SpriteClips[7].w = SPRITEH;
		SpriteClips[7].h = SPRITEH;

	}

	
		return true;
}
void Game::ClearGlobalRenderer() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
	
	std::cout << "gRenderer cleared!" << std::endl;
}

void Game::LevelTransition() {
	currentLevel = 2;
	unlockedLevel = currentLevel;
	WhichMap = LEVELTWOMAP;
	P1.ResetPos(currentLevel);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);
	AllTexture.loadFromRenderedText(gRenderer, NEWLEVEL, "Level Two",BlackColor,gfont);
	AllTexture.render(gRenderer, NEWLEVEL, (screenW - AllTexture.getWidth(NEWLEVEL)) / 2, (screenH - AllTexture.getHeight(NEWLEVEL)) / 2);
	SDL_RenderPresent(gRenderer);
	SDL_Delay(5000);
}
void Game::GameOver() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
	AllTexture.loadFromRenderedText(gRenderer, GAMEOVERTEXT, "Game Over", WhiteColor, gfont);
	AllTexture.render(gRenderer, GAMEOVERTEXT, (screenW - AllTexture.getWidth(GAMEOVERTEXT)) / 2, (screenH - AllTexture.getHeight(GAMEOVERTEXT)) / 2);
	SDL_RenderPresent(gRenderer);
	SDL_Delay(5000);
	//master reset
	P1.reset();
	BOU.reset();
	MM.reset();
	reset();
	loadWelcomeText();
}


bool gameSounds::initSounds() { 
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { //chunksize option here
		std::cout << "SDL mixer couldn't be initialized. Error code: " <<Mix_GetError() << std::endl;
	}
	return true;
}
bool gameSounds::loadAllSounds() {
	gMusic = Mix_LoadMUS("sounds/softbeat/soft-beat.mp3");
	if (gMusic == NULL) {
		std::cout << "Could not load global music. Error code: " << Mix_GetError() << std::endl;
	}

	//chunk loading procedure left to be completed
	//chunk_name = Mix_LoadWav("path");
	return true;
}
void gameSounds::closeSounds() {
	Mix_FreeMusic(gMusic); //freeing the global music
	gMusic = NULL; //This necessary I guess
	Mix_Quit(); //Ending this part from here instead of doing it in one messed up conglomeration

}
void gameSounds::playGlobalMusic() {
	Mix_PlayMusic(gMusic, -1);
}

void Player::render(SDL_Renderer* gRenderer, SDL_Rect* currentClip) {
	//Show the dot
	AllTexture.render(gRenderer, GSPRITESHEETTEXTURE, mPosX, mPosY, currentClip, 0.0f, 0, FlipVal);
}
void Player::RenderObstacles(SDL_Renderer* gRenderer,int currentLevel) {

	//going to use a large ass image, cant be bothered with tileset mapping rn
	if (currentLevel == 1) {
		//SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); //green
		for (int i = GOODPLATFORMONE;i <= FLOORTHREEGOODPLATFORMTWO;i++) {
			AllTexture.render(gRenderer, WALLTEXTURE, Obstacle[i].x, Obstacle[i].y, &Obstacle[i]);
			//SDL_RenderFillRect(gRenderer, &Obstacle[i]);
		}
		//SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); //red
		for (int i = BADPLATFORMONE;i <= FLOORTWOBADPLATFORMTWO;i++) {
			AllTexture.render(gRenderer, LAVA, Obstacle[i].x, Obstacle[i].y, &Obstacle[i]);
			//SDL_RenderFillRect(gRenderer, &Obstacle[i]);
		}
	}

	//level two seperate obstacle rendering
	if (currentLevel == 2) {
		AllTexture.render(gRenderer, WALLTEXTURE, Obstacle[LVLTWOGOODPLATFORMTHREE].x, Obstacle[LVLTWOGOODPLATFORMTHREE].y, &Obstacle[LVLTWOGOODPLATFORMTHREE]);
	}
}

void Checkpoint::renderCheckPoint(int coinframe,int Desti) { //keep this in playerhandler for now
	SDL_Rect* CurrentCoinFrame = &CheckpointSprites[coinframe / 6]; //issue here ig
	AllTexture.render(gRenderer, CHECKCOIN, ckPoint[Desti].x, ckPoint[Desti].y, CurrentCoinFrame);
}

void Boulder::renderBoulder(int boulderframe) {
	AllTexture.render(gRenderer, BOULDER, BoulderRect.x, BoulderRect.y, &BoulderSprites[boulderframe / 8]);
}
void Game::RenderAnimatedBoulder() {
	BOU.renderBoulder(boulderframe);
	++boulderframe;
	//Cycle animation
	if (boulderframe / 8 >= STATIONARY_ANIMATION_FRAMES) //boulder frame = animated sheet frames
	{
		boulderframe = 0;
	}
}

void MainMenu::RenderMenu() {
	AllTexture.render(gRenderer, MAINMENUANIM, 0, 0, &rectSrc);
	SDL_RenderPresent(gRenderer);
}
void MainMenu::renderUnlockedLevel(){
	AllTexture.render(gRenderer, LEVELUNLOCKED, 0, 0, &unlockedSrc);
	SDL_RenderPresent(gRenderer);
	}
void MainMenu::showHighScores() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
	AllTexture.loadFromRenderedText(gRenderer, HIGHSCORE, "HIGHSCORE:", WhiteColor, gfont);
	AllTexture.render(gRenderer, HIGHSCORE, 0, 0);
	SDL_RenderPresent(gRenderer);
	SDL_Delay(7000); //freeze this bih for 7 seconds instead of having to deal with bools
}
void MainMenu::toggleLRVal() {
	P1.switchLRlock();
}