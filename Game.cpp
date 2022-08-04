#include <Game.h> 
#include <chrono>
#include <thread>
#include <iomanip>

#define SPRITEW 60
#define SPRITEH 59
#define TIMERWIDTH 360


using namespace std; 

//global chrono delay
std::chrono::milliseconds timespan(500); 


//temporary global function
TTF_Font* gfont;
//TTF_Font* globalFont; //Global font declaration, works here?
SDL_Renderer* gRenderer;
textureClass gTextTexture,gSpriteSheetTexture,gTimer; //maybe this can go inside the Game class?
textureClass WelcomeOBH, PressStart;
textureClass BigLogo;

gameSounds MegaSoundObj;

Mix_Music* gMusic;
std::stringstream timeInText; ///for the timer text
SDL_Color BlackColor = { 0,0,0 };


Game::Game() { //constructor
	screenW = 1140;
	screenH = 600; //screen width and height initialization
	gamestate = GameState::PLAY;
	gMusic = NULL;
	//gfont = NULL;
	WhichMap = WELCOMESCREEN;
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
	init("One Button Hero",50,50,screenW,screenH,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE ); //CAN DO FULLSCREEN MODE HERE
}
void Game::init(const char *title, int x, int y, int w, int h, Uint32 flags) {
	//initialize everything
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL. Error code: "<<SDL_GetError() << endl;
	}
	else {
		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == -1) {
			cout << "Unable to initialize jpg and png" << endl;
		}
		//initializing SDL ttf
		if (TTF_Init() == -1)
		{
			cout << "Unable to initialize SDL TTF. Error code:" <<TTF_GetError()<<endl;
		}
		
		window=SDL_CreateWindow(title, x, y, w, h,flags);
		if (window == NULL) {
			cout << "Unable to create window! Error code:" << SDL_GetError() << endl;
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
				draw();
			}
			break;

		

			//we shall nest the map conditionals inside here so that global keypresses still remain functional
			//switch (WhichMap) {
			//case WELCOMESCREEN:
				//resizing restricted to the welcome screen for now, but should be doable if we set variables based on screensurface
				//if (evnt.type == SDL_WINDOWEVENT_RESIZED) {
		case SDL_WINDOWEVENT:
			if (evnt.window.event == SDL_WINDOWEVENT_RESIZED) {
				screenSurface = SDL_GetWindowSurface(window);

				//lets see if this shit works
				//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, reinterpret_cast<char*>(0));
				//SDL_RenderSetLogicalSize(gRenderer, 800, 400); ///<-----
				//loadWelcomeText();
				//SDL_RenderPresent(gRenderer);
				//loadWelcomeText(); //issue being that the text is reset when resized AAA
			//	SDL_UpdateWindowSurface(window);
				cout << "Dimensions " << screenSurface->w << " x " << screenSurface->h << endl;
				cout << "Window resized!" << endl;
				break;
			}
				//}
				//break;
			//}
			
		}
	}
}
void Game::draw() {

	
	// Clear screen
	MapRunning = true;
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//Render current frame
	SDL_Rect* currentClip = &SpriteClips[frame / 8]; //adjust framerate
	gSpriteSheetTexture.render(gRenderer,(screenW - currentClip->w) / 2, (screenH - currentClip->h) / 2, currentClip);

	

	timeInText.str(""); //Lets try without his X, TURNS OUT THIS IS FOR CLEARING THE PAST TEXT?
	timeInText << "Time passed: " << (SDL_GetTicks() - startTime) /1000<<setw(5)<<" s";
	
	gTimer.loadFromRenderedText(gRenderer, timeInText.str().c_str(), BlackColor, gfont);
	gTimer.render(gRenderer,screenW - TIMERWIDTH, 0);

	//Update screen
	SDL_RenderPresent(gRenderer);

	//Go to next frame
	++frame;

	//Cycle animation
	if (frame / 8 >= STATIONARY_ANIMATION_FRAMES)
	{
		frame = 0;
	}
	//dry run, replace with wrapped class pls.
//	SDL_FillRect(screenSurface,NULL,SDL_MapRGB(screenSurface->format,0,0,0));
	//SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);
//	SDL_UpdateWindowSurface(window);
	//see if it runs without the updatewindow

	//for now lets run the welcome text from this place
	 //will change the object name later on, might switch into an array with enum integrated
}


void Game::loadWelcomeText() {
	

	//sorted that we dont need to resize and can use point size instead
	WelcomeOBH.render(gRenderer,(screenW-WelcomeOBH.getWidth())/2, (screenH-WelcomeOBH.getHeight()) / 2);  //default argument enabled
	//positioned such as the welcome text is rendered below the press to start
	PressStart.render(gRenderer,(screenW-PressStart.getWidth()) / 2, (screenH-PressStart.getHeight()) / 2 + WelcomeOBH.getHeight() + 5);
	//giving some clearance
	//SDL_RenderCopy(gRenderer, mTexture, NULL, &WelcomeText); //<here it is being rendered
	SDL_RenderPresent(gRenderer);
	

}
void Game::LoadAppIcon() {
	SDL_SetWindowIcon(window, IMG_Load("images/ICON.png"));
	cout << "Load app icon launched" << endl;
	LoadingScreen();
	//loading screen
}
void Game::LoadingScreen() {
	if (!launchedLoadingScreen) {
		//temporary clipping solution
		SDL_Rect clipper;
		clipper = { screenSurface->w / 3,screenSurface->h / 6,screenSurface->w / 3,screenSurface->w / 3 };
		BigLogo.renderResized(gRenderer,&clipper); //default args

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

/*
bool textureClass::loadFromFile(gRenderer,string path) {

	//free();
	//NOT defined yet

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image %s! SDL_image Error: " << path.c_str() << IMG_GetError() << endl;
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) //check this part for error, gave error although working last time, BUT NEED THIS TO GET DIMENSIONS
		{
			cout<<"Unable to create texture from % s!SDL Error : " << path.c_str() << SDL_GetError()<<endl;
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}
void textureClass::render(gRenderer,int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
	//REPLACED SRCRECT FROM CLIP TO NULL
	//USE SDL_FLIP_NONE for no flip
	//USE NULL for center to set rotating point at center of the texture.
}
void textureClass::renderResized(gRenderer,SDL_Rect* Desti, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(gRenderer, mTexture, NULL, Desti, angle, center, flip);
	SDL_RenderPresent(gRenderer);
}
int textureClass::getWidth() {
	return mWidth;
}
int textureClass::getHeight() {
	return mHeight;
}
bool textureClass::loadFromRenderedText(gRenderer,std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	//free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gfont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		cout<<"Unable to render text surface! SDL_ttf Error: "<< TTF_GetError()<<endl;
	}
	else
	{
		//Create texture from surface pixels
		//SDL_Renderer* lrenderer;
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			cout<<"Unable to create texture from rendered text! SDL Error: "<< SDL_GetError()<<endl;
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w/2;
			mHeight = textSurface->h/2;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);

		//Substitute rendering text here to using a seperate render function so that it is more reusable
	}

	//Return success
	return mTexture != NULL;
} */

bool Game::loadMedia() {

	//fonts 
	gfont = TTF_OpenFont("ka1.ttf", 72); //ptsize is size of font 

	if (gfont == NULL) {
		cout << "Failed to load the ttf file. Error code" << TTF_GetError() << endl;
	}

	SDL_Color textColor = { 255, 255, 255 };

	if (!WelcomeOBH.loadFromRenderedText(gRenderer, "Welcome to One Button Hero", textColor, gfont)) {
		cout << "Failed to render texture." << endl;
	}
	gfont = TTF_OpenFont("ka1.ttf", 48);
	if (!PressStart.loadFromRenderedText(gRenderer, "Press any button to start.", textColor, gfont)) {
		cout << "Failed to render texture." << endl;
	}

	//timer
	



	//images

	BigLogo.loadFromFile(gRenderer, "images/LOGO.png");

	//SOUND AREA


	if (!MegaSoundObj.initSounds()) {
		cout << "Unable to initialize the mixer!!!!" << endl;
	}
	if (!MegaSoundObj.loadAllSounds()) {
		cout << "Unable to load all sounds from file!!!" << endl;
	}

	if (!gSpriteSheetTexture.loadFromFile(gRenderer,"nicepng60pxw.png"))
	{
		printf("Failed to load walking animation texture!\n");
		//success = false;
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
	
	cout << "gRenderer cleared!" << endl;
}

bool gameSounds::initSounds() { 
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { //chunksize option here
		cout << "SDL mixer couldn't be initialized. Error code: " <<Mix_GetError() << endl;
	}
	return true;
}
bool gameSounds::loadAllSounds() {
	gMusic = Mix_LoadMUS("sounds/softbeat/soft-beat.mp3");
	if (gMusic == NULL) {
		cout << "Could not load global music. Error code: " << Mix_GetError() << endl;
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