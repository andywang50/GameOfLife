#include "MainGame.h"
#include <iostream>
//#include <SDL/SDL.h>
//#include <SDL/SDL_main.h>
//#include <SDL/SDL_ttf.h>

#ifdef WIN32
#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDL_main.lib")
#endif
using std::cerr;
using std::endl;
int main(int argc, char** argv) {
	MainGame mainGame;
	mainGame.run();
	return 0;
}