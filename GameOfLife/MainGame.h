#ifndef _MAINGAME_H
#define _MAINGAME_H 1
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include "Cell.h"
#include <SDL/SDL_ttf.h>
#include "board.h"
#include "Button.h"
#include <fstream>
enum class GameState{PLAY, PAUSE, EXIT};
class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	friend Uint32 my_callbackfunc(Uint32 interval, void *param);

private:
		board cell_board;

		SDL_Window* _window;
		SDL_Renderer* renderer;
		void set_renderer_color(SDL_Color color);

		SDL_Window* _graphW;
		SDL_Renderer* _graphR;
		int _graphscreenWidth;
		int _graphscreenHeight;

		SDL_Window* _phasegraphW;
		SDL_Renderer* _phasegraphR;
		int _phasegraphscreenWidth;
		int _phasegraphscreenHeight;

		Uint32 mWindowID;
		int _screenWidth;
		int _screenHeight;
		
		
		void initSystems();
		void processInput();
		void gameLoop();
		void nextStage();
		GameState _gameState;
		SDL_TimerID my_timer_id;

		void draw();
		void clear();
		void restart();
		void resume();
		void pause();
		void change_party();


		int num_rect_horizontal;
		int num_rect_vertical;
		int y_max;
		int x_max;
		int rect_width;
		int start_pt_x;
		int start_pt_y;
		int margin;

		int num_func_button;
		int func_button_width;
		int func_button_height;
		int func_button_margin;
		int func_button_left;
		int func_button_top;
		Button* button_array;
		int highlight_button;
		void disable_button(ButtonFunc);
		void enable_button(ButtonFunc);
		
		SDL_RWops *forward_image;
		SDL_RWops *pause_image;
		SDL_Surface* button_surface[2];
		SDL_Texture* button_texture;

		TTF_Font* title_font;
		SDL_Surface* message_surface;
		SDL_Texture* message_texture;
		
		cellParty current_party;

		int graphx;
		void draw_timegraph();
		SDL_Point* Party1;
		SDL_Point* Party2;
		int graphx_step;

		void reset_point_arrays();
		void clear_graphs();
		void draw_phasegraph(int,int);
		int old_phase_x;
		int old_phase_y;

		std::ofstream fout;
};


#endif