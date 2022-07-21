#include <Game.h> 
using namespace std; 
Game::Game() { //constructor
	screenW = 800;
	screenH = 400; //screen width and height initialization
	gamestate = GameState::PLAY;
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
		window=SDL_CreateWindow(title, x, y, w, h,flags);
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
	SDL_Rect 
}