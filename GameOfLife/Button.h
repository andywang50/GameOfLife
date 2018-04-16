#ifndef _BUTTON_H
#define _BUTTON_H 1
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "Color.h"
enum class ButtonFunc { PLAY, NEXT, CHANGEPARTY, CLEAR, RESTART, QUIT, NONE };

class Button {
public:
	friend class MainGame;
	SDL_Color get_color() const;
	//Button();
	//~Button();
	//Button(const Button& b);
	//Button& operator = (const Button& b);
private: 
	ButtonFunc _id;
	bool hover;
	bool disabled;

	//int _specialFeature;
//	SDL_Surface* image_surface; 
	SDL_Surface* msg_surface;
//	SDL_Texture* image_texture;
	SDL_Texture* msg_texture;
	
};
#endif
