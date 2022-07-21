#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

enum class GameState {
	PLAY,
	EXIT
};
enum class TextureId {
	BACKGROUND_TEXTURE,
	TOTAL_TEXTURES
};
enum class RectangleId {
	BACKGROUND_RECT,
	TOTAL_RECTANGLES
};
class Game {
public:
	Game();
	~Game();
	void run();
	void init(const char *, int,int,int, int,Uint32);
	void draw();
	void gameLoop();
	void handleEvents();
private:
	int screenW, screenH;
	SDL_Window* window;
	SDL_Renderer* renderer;
	GameState gamestate;
};
class textureClass{
private:
	int _textureW,_textureH;
public:
	textureClass();
	void DisplayLoadedMessage();
};
class surfaceClass{};
class rectangleClass {
private:
	SDL_Rect Rect;
public:
	
};