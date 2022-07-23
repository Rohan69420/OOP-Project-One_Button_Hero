#include <Game.h> 
using namespace std; 
//temporary global declaration

//temporary global function
TTF_Font* gfont;
//TTF_Font* globalFont; //Global font declaration, works here?
SDL_Renderer* gRenderer;
textureClass gTextTexture;

Game::Game() { //constructor
	screenW = 800;
	screenH = 400; //screen width and height initialization
	gamestate = GameState::PLAY;
	//gfont = NULL;
}
Game::~Game() { //destructor
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Game::run() {
	init("One Button Hero",50,50,screenW,screenH,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
}
void Game::init(const char *title, int x, int y, int w, int h, Uint32 flags) {
	//initialize everything
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL. Error code: "<<SDL_GetError() << endl;
	}
	else {
		//initializing SDL ttf
		if (TTF_Init() == -1)
		{
			cout << "Unable to initialize SDL TTF. Error code:" <<TTF_GetError()<<endl;
		}
		//TTF_Font* globalFont;
		// 
		//loading global ttf files
		//textureClass gTextTexture;
		loadGlobalText();
		window=SDL_CreateWindow(title, x, y, w, h,flags);
		if (window == NULL) {
			cout << "Unable to create window! Error code:" << SDL_GetError() << endl;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		}
		//SDL_Delay(2000);
		gameLoop();

	}
}
void Game::gameLoop() {
	draw();
	while (gamestate != GameState::EXIT) {
		handleEvents();
	}
}
void Game::handleEvents() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			gamestate = GameState::EXIT;
			break;
		}
	}
}
void Game::draw() {
	//dry run, replace with wrapped class pls.
	SDL_FillRect(screenSurface,NULL,SDL_MapRGB(screenSurface->format,0,0,0));
	//SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
	//see if it runs without the updatewindow

	//for now lets run the welcome text from this place
	 //will change the object name later on, might switch into an array with enum integrated
}

//texture class imp-temp definition for loading texture
bool textureClass::loadFromRenderedText(std::string textureText, SDL_Color textColor)
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
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

void Game::loadGlobalText() {
	gfont = TTF_OpenFont("ka1.ttf", 28);
	if (gfont == NULL) {
		cout << "Failed to load the ttf file. Error code" << TTF_GetError() << endl;
	}
	else {
		SDL_Color textColor = { 255, 255, 255 };
		//textureClass gTextTexture;
		if (!gTextTexture.loadFromRenderedText("Welcome to One Button Hero", textColor)) {
			cout << "Failed to render texture." << endl;
		}
	}

}
