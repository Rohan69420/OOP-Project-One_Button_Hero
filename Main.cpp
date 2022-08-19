#include "Game.h"
#include <Windows.h>



int main(int argc, char* argv[]) {
	HWND windowHandle = GetConsoleWindow();
	//ShowWindow(windowHandle, SW_HIDE);

	Game game;
	//textureClass gTextTexture;
	game.run();
	return 0;
}