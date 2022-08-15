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
std::stringstream timeInText; ///for the timer text
SDL_Color BlackColor = { 0,0,0 };

Player P1;

Game::Game() { //constructor
	gamestate = GameState::PLAY;
	gMusic = NULL;
	//gfont = NULL;
	WhichMap = FIRSTMAP;
	//STATIONARY_ANIMATION_FRAMES = 8;
	frame = 0;
	//SpriteClips = new SDL_Rect[STATIONARY_ANIMATION_FRAMES]; //LOOKS LIKE IT WORKED

	startTime = 0;
	tickStarted = false;

	launchedLoadingScreen = false;
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
	while (gamestate != GameState::EXIT) {
		handleEvents();
		//P1.move(); //what if move is shifted inside handle events?
		if (MapRunning) {
			if (!tickStarted) {
				startTime = SDL_GetTicks();
				tickStarted = true;
			}
			draw();
		}
	}
}
void Game::handleEvents() {
	SDL_Event evnt;
	P1.positiveGravity();
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			gamestate = GameState::EXIT;
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
			else {
				P1.handleEvent(evnt);
				P1.move();
				draw();
			}
			break;

			case SDL_KEYUP:
				P1.handleEvent(evnt);
				P1.move();
				draw();
			break;

		//removed the resizable part altogether
			
		}
	}
}
void Game::draw() {

	
	// Clear screen
	MapRunning = true;
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//render background image i guess
	AllTexture.render(gRenderer, WhichMap, 0, 0);

	//Render current frame
	SDL_Rect* currentClip = &SpriteClips[frame / 8]; //adjust framerate
	P1.render(currentClip);
	/*AllTexture.render(gRenderer,GSPRITESHEETTEXTURE,(SpriteLocationX - currentClip->w) / 2, (SpriteLocationY - currentClip->h) / 2, currentClip);*/

	

	timeInText.str(""); //Lets try without his X, TURNS OUT THIS IS FOR CLEARING THE PAST TEXT?
	timeInText << "Time passed: " << (SDL_GetTicks() - startTime) /1000<<setw(5)<<" s";
	
	AllTexture.loadFromRenderedText(gRenderer,GTIMER, timeInText.str().c_str(), BlackColor, gfont);
	AllTexture.render(gRenderer,GTIMER,screenW - TIMERWIDTH, 0);

	//Update screen
	SDL_RenderPresent(gRenderer);

	//Go to next frame
	++frame;

	//Cycle animation
	if (frame / 8 >= STATIONARY_ANIMATION_FRAMES)
	{
		frame = 0;
	}
	
}


void Game::loadWelcomeText() {
	

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
	



	//images

	if (!AllTexture.loadFromFile(gRenderer, BIGLOGO, "images/LOGO.png")) {
		std::cout << "Failed to load the biglogo image!" << std::endl;
	}
	

	//SOUND AREA


	if (!MegaSoundObj.initSounds()) {
		std::cout << "Unable to initialize the mixer!!!!" << std::endl;
	}
	if (!MegaSoundObj.loadAllSounds()) {
		std::cout << "Unable to load all sounds from file!!!" << std::endl;
	}

	if (!AllTexture.loadFromFile(gRenderer,GSPRITESHEETTEXTURE,"nicepng60pxw.png"))
	{
		std::cout<<"Failed to load walking animation texture!\n"<<std::endl;
		//success = false;
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

Player::Player() {
	mPosX = 0;
	mPosY = 0;
	mVelX = 0;
	mVelY = 0;
	
	//facing directions
	FlipVal = SDL_FLIP_NONE;
	DF = RIGHT; //persisting issue that it gets reset to this variable once scope ends but we dont need to tackle this issue in our original plan
}
void Player::handleEvent(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)		///<<<<<<<<	-	-	- need key repeat because only one press is enuff
	{									////////<<<< meaning that even if you are holding key it will still by 10fps vel
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= DOT_VEL; break;
		case SDLK_DOWN: mVelY += DOT_VEL; break;
		case SDLK_LEFT: mVelX -= DOT_VEL; 
			if (DF == RIGHT) {
				DF = LEFT;
				FlipVal= SDL_FLIP_HORIZONTAL;
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
				FlipVal=SDL_FLIP_NONE;
			}
			break;
		}
	}
	//Release
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)		//<<<<<<<<< reverse velocity?
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN: mVelY -= DOT_VEL; break;
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
void Player::render(SDL_Rect *currentClip) {
	//Show the dot
	AllTexture.render(gRenderer, GSPRITESHEETTEXTURE, mPosX, mPosY, currentClip,0.0f,0,FlipVal);
}
void Player::positiveGravity() {
	if (!Collision()) {
		mPosY += 5;
	}
}
bool Player::Collision() {
	if ((mPosX < 0) || (mPosY < 0) || (mPosX + DOT_WIDTH) > screenW || (mPosY + DOT_HEIGHT) > screenH) {
		return true;
	}
	return false;
}