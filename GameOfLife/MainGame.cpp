#include "MainGame.h"
#include "Cell.h"
#include <iostream>
#include <string>
#include <time.h>       /* time */
#include <string>

using std::cerr;
Uint32 my_callbackfunc(Uint32 interval, void *param);


void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
}
MainGame::MainGame() {
	_window = nullptr;
	renderer = NULL;

	_screenHeight = 768;
	_screenWidth = 1536;
	_gameState = GameState::PAUSE;

	_graphscreenWidth = 800;
	_graphscreenHeight = 768;
	num_rect_horizontal = 80;
	num_rect_vertical = 40;
	y_max = num_rect_vertical - 1;
	x_max = num_rect_horizontal - 1;
	rect_width = 15;
	start_pt_x = 50;
	start_pt_y = 50;
	margin = 1;

	num_func_button = 6;
	func_button_width = 150;
	func_button_height = 80;
	func_button_margin = (num_rect_vertical * (rect_width + margin) - func_button_height) / (num_func_button - 1) - func_button_height;
	func_button_left = (start_pt_x + num_rect_horizontal * (rect_width + margin) + _screenWidth - func_button_width) / 2;
	func_button_top = start_pt_y;


	cell_board = board(y_max, x_max);
	button_array = new Button[num_func_button];
	highlight_button = (int)(ButtonFunc::NONE);

	button_surface[0] = NULL;
	button_surface[1] = NULL;

	button_texture = NULL;
	message_surface = NULL;
	message_texture = NULL;
	forward_image = NULL;
	pause_image = NULL;
	//SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	current_party = (cellParty)1;
	graphx = 0;
	graphx_step = 2;
	Party1 = new SDL_Point[_graphscreenWidth/graphx_step+1];
	Party2 = new SDL_Point[_graphscreenWidth / graphx_step+1];

	_phasegraphscreenWidth = 800;
	_phasegraphscreenHeight = 768;

}
MainGame::~MainGame()
{
	delete[] button_array;
	_window = NULL;
	renderer = NULL;
	SDL_DestroyTexture(button_texture);
	SDL_DestroyTexture(message_texture);
}
void MainGame::run() {
	srand(time(NULL));
	
	initSystems();
	
	gameLoop();
	
	fout.close();

	
}

void MainGame::initSystems() {

	TTF_Init();

	fout.open("gameoflife.txt");

	SDL_Init(SDL_INIT_EVERYTHING);		//initialize sdl
	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL); //title, position of x and y, screen width, height
	mWindowID = SDL_GetWindowID(_window);

	_graphW = SDL_CreateWindow("timegraph", 10, SDL_WINDOWPOS_CENTERED, _graphscreenWidth, _graphscreenHeight, SDL_WINDOW_OPENGL);
	_graphR = SDL_CreateRenderer(_graphW, -1, SDL_RENDERER_ACCELERATED);

	_phasegraphW = SDL_CreateWindow("phasegraph",900, SDL_WINDOWPOS_CENTERED, _phasegraphscreenWidth, _phasegraphscreenHeight, SDL_WINDOW_OPENGL);
	_phasegraphR = SDL_CreateRenderer(_phasegraphW, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(_phasegraphR, 255, 0, 0, 255);

	//SDL_Window* _window1 = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 200, 200, SDL_WINDOW_OPENGL); //title, position of x and y, screen width, height

	if (_window == nullptr) {
		fatalError("SDL Window cannot be created!");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {

		fatalError("SDL_GL context could not be created!");
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	cell_board.setNeighbors();
	renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	// Set render color to red ( background will be rendered in this color )
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	SDL_SetRenderDrawColor(_graphR, 255, 255, 255, 255);
	// Clear winow
	SDL_RenderClear(renderer);
	title_font = TTF_OpenFont("segoeprb.ttf", 24); //this opens a font style and sets a size
	message_surface = TTF_RenderText_Solid(title_font, "Welcome to the WICKED Conway's Game of LIIIIIIIIIFE!!!", { 255,255,255 }); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	message_texture = SDL_CreateTextureFromSurface(renderer, message_surface); //now you can convert it into a texture

	for (int i = 0; i < num_func_button; i++) {
		button_array[i]._id = (ButtonFunc)(i);
		button_array[i].hover = false;
		button_array[i].disabled = false;
		if ((ButtonFunc)i != ButtonFunc::PLAY) {
			std::string msg;
			if ((ButtonFunc)i == ButtonFunc::CHANGEPARTY) {
				int num = (int)current_party;
				msg = "PARTY" + std::to_string(num);
				
			}
			else if ((ButtonFunc)i == ButtonFunc::NEXT) {
				msg = "NEXT";
			
			}
			else if ((ButtonFunc)i == ButtonFunc::RESTART) {
				msg = "RESTART";

			}
			else if ((ButtonFunc)i == ButtonFunc::CLEAR) {
				msg = "CLEAR";

			}
			else if ((ButtonFunc)i == ButtonFunc::QUIT) {
				msg = "QUIT";

			}
			button_array[i].msg_surface = TTF_RenderText_Solid(title_font, msg.c_str(), { 0,0,0 });
			button_array[i].msg_texture = SDL_CreateTextureFromSurface(renderer, button_array[i].msg_surface);
		}
		else {
			button_array[i].msg_surface = NULL;
			button_array[i].msg_texture = NULL;
		}
	}
	// load images
	pause_image = SDL_RWFromFile("left.png", "rb");
	forward_image = SDL_RWFromFile("right.png", "rb");
	button_surface[0] = IMG_LoadPNG_RW(forward_image);
	button_surface[1] = IMG_LoadPNG_RW(pause_image);
	button_texture = SDL_CreateTextureFromSurface(renderer, button_surface[0]); //create texture
	
	draw();
	
	return;
}
void MainGame::processInput() {
	SDL_Event evnt;
	Uint32 delay = (20 / 10) * 10;  /* To round it down to the nearest 10 ms */
	

	//my_timer_id = SDL_AddTimer(delay, my_callbackfunc, NULL);
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			return;
		}
			switch (evnt.type) {
				case SDL_QUIT:
					_gameState = GameState::EXIT;
					return;

				case SDL_MOUSEMOTION: {
					//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
					if (evnt.window.windowID == mWindowID) {
						int x = evnt.motion.x;
						int y = evnt.motion.y;
						int x_coord = floor(1.0*(x - start_pt_x) / (rect_width + margin));
						int y_coord = floor(1.0*(y - start_pt_y) / (rect_width + margin));

						if ((_gameState == GameState::PAUSE)) {
							cell_board.highlight(y_coord, x_coord);
							draw();
						}

						x_coord = floor(1.0*(x - func_button_left) / (func_button_width));
						y_coord = floor(1.0*(y - func_button_top) / (func_button_height + func_button_margin));
						if ((x_coord >= 0) && (y_coord >= 0) && (x_coord < 1) && (y_coord <= num_func_button - 1)) {
							if (1.0 * (y - func_button_top) / (func_button_height + func_button_margin) - y_coord <= 1.0* (func_button_height) / (func_button_height + func_button_margin)) {
								button_array[y_coord].hover = true;
								highlight_button = y_coord;
							}
							else {
								button_array[highlight_button].hover = false;
								highlight_button = (int)(ButtonFunc::NONE);

							}
						}
						else {
							button_array[highlight_button].hover = false;
							highlight_button = (int)(ButtonFunc::NONE);

						}
						break;
					}
				}
				case SDL_MOUSEBUTTONDOWN: {
					if (evnt.window.windowID == mWindowID) {
						int x = evnt.motion.x;
						int y = evnt.motion.y;
						int x_coord = floor(1.0*(x - start_pt_x) / (rect_width + margin));
						int y_coord = floor(1.0*(y - start_pt_y) / (rect_width + margin));


						if ((x_coord >= 0) && (y_coord >= 0) && (x_coord <= x_max) && (y_coord <= y_max)) {
							if (_gameState == GameState::PAUSE) {
								cell_board.changeCellState(y_coord, x_coord, current_party);
								draw();
							}
						}
						else {
							x_coord = floor(1.0*(x - func_button_left) / (func_button_width));
							y_coord = floor(1.0*(y - func_button_top) / (func_button_height + func_button_margin));
							if ((x_coord >= 0) && (y_coord >= 0) && (x_coord <= 1) && (y_coord <= num_func_button - 1)) {
								if (1.0 * (y - func_button_top) / (func_button_height + func_button_margin) - y_coord <= 1.0* (func_button_height) / (func_button_height + func_button_margin)) {
									ButtonFunc id = (ButtonFunc)y_coord;
									switch (id) {
									case ButtonFunc::PLAY: {
										SDL_Event event;
										event.type = SDL_KEYDOWN;
										if (_gameState == GameState::PAUSE) event.key.keysym.sym = SDLK_RIGHT;
										else event.key.keysym.sym = SDLK_LEFT;
										event.window.windowID = mWindowID;
										SDL_PushEvent(&event);
										break;
									}
									case ButtonFunc::NEXT: {
										if (_gameState == GameState::PAUSE) {
											SDL_Event event;
											event.type = SDL_KEYDOWN;
											event.key.keysym.sym = SDLK_SPACE;
											event.window.windowID = mWindowID;

											SDL_PushEvent(&event);
										}
										break;
									}
									case ButtonFunc::CHANGEPARTY: {
										if (_gameState == GameState::PAUSE) {
											SDL_Event event;
											change_party();
										}
										break;
									}
									case ButtonFunc::CLEAR: {
										if (button_array[(int)(ButtonFunc::CLEAR)].disabled == false) {

											clear();
										}
										break;
									}
									case ButtonFunc::RESTART: {
										//if (_gameState == GameState::PAUSE) {
										if (button_array[(int)(ButtonFunc::RESTART)].disabled == false) {

											restart();
										}
										break;
									}
									case ButtonFunc::QUIT: {
										SDL_Event event;
										event.type = SDL_QUIT;
										SDL_PushEvent(&event);
										break;
									}
									default: {

										break;
									}
									}
								}
							}
						}
					}
					break;
				}

				case SDL_KEYDOWN: {
					switch (evnt.key.keysym.sym) {
					case SDLK_SPACE: {
						//if (_gameState == GameState::PAUSE) {
						if (button_array[(int)(ButtonFunc::NEXT)].disabled == false){
							nextStage();

						}
						break;
					}
					case SDLK_RIGHT: {
						if (_gameState == GameState::PAUSE) {
						
							//_gameState = GameState::PLAY;
							resume();
							my_timer_id = SDL_AddTimer(delay, my_callbackfunc, &(this->mWindowID));

						}
						
						break;
					}
					case SDLK_LEFT: {
						if(_gameState == GameState::PLAY) {
							pause();
							//_gameState=GameState::PAUSE;
							SDL_RemoveTimer(my_timer_id);
						}
						break;
					}
					case SDLK_r: {
						if (_gameState == GameState::PAUSE) {
							restart();
						}
						break;
					}
					case SDLK_c: {
						if (_gameState == GameState::PAUSE) {
							clear();
						}
						break;
					}
					case SDLK_ESCAPE:{
						SDL_Event temp;
						temp.type = SDL_QUIT;
						SDL_PushEvent(&temp);
						break;
					}
					default:
						break;
					}
					break;
				}

				case SDL_USEREVENT: {

					if (_gameState == GameState::PLAY) {
						nextStage();
					}
					break;
				}

			}
		
	}
}
void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		processInput();
 
	}
}
void MainGame::nextStage() {
	cell_board.calc_nextStage();
	cell_board.update_nextStage();
	draw();
	draw_timegraph();
}
void MainGame::draw() {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);


	// Clear winow
	SDL_RenderClear(renderer);
	// drawing cells
	for (int i = 0; i < num_rect_vertical; i++) {
		for (int j = 0; j < num_rect_horizontal; j++) {
			SDL_Rect r;
			r.x = start_pt_x + j*(rect_width + margin);
			r.y = start_pt_y + i*(rect_width + margin);
			r.w = rect_width;
			r.h = rect_width;
			cell* temp = cell_board[i][j];
			
			set_renderer_color(temp->get_color(_gameState == GameState::PAUSE));

			SDL_RenderFillRect(renderer, &r);
			
		}
	}
	
	// drawing button rectangles
	for (int i = 0; i < num_func_button; i++) {
		SDL_Rect r;
		r.x = func_button_left;
		r.y = func_button_top + i*(func_button_margin+func_button_height);
		r.w = func_button_width;
		r.h = func_button_height;
		Button* temp = &button_array[i];
		set_renderer_color(temp->get_color());
		SDL_RenderFillRect(renderer, &r);
		if (temp->msg_texture != NULL) SDL_RenderCopy(renderer, temp->msg_texture, NULL, &r);
	}
	//SDL_RenderPresent(renderer);
	
	SDL_Rect button_pos; //the rectangle in which the image is put
	button_pos.w = func_button_width;
	button_pos.h = func_button_height;
	button_pos.x = func_button_left;
	button_pos.y = func_button_top;

	
	//if (button_texture) std::cout << "Success" << std::endl;
	//else std::cout << "Failed" << std::endl;
	
	SDL_RenderCopy(renderer, button_texture, NULL, &button_pos);
	
	
	
	SDL_Rect Message_rect; //create a rect
	Message_rect.w = 1000; // controls the width of the rect
	Message_rect.h = 50; // controls the height of the rect

	Message_rect.x = (_screenWidth - Message_rect.w)/2;  //controls the rect's x coordinate 
	Message_rect.y = (_screenHeight - Message_rect.h); // controls the rect's y coordinte
	

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &Message_rect); 
	SDL_RenderCopy(renderer, message_texture, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
	
	SDL_RenderPresent(renderer);
	
	//if (title_font) std::cout << "Success" << std::endl;
	//else std::cout << "Failed" << std::endl;
	
	
}
Uint32 my_callbackfunc(Uint32 interval,  void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;


	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	//userevent.data1 = &nextStage;
	//userevent.data2 = param;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	event.window.windowID = *static_cast<Uint32*>(param);

	SDL_PushEvent(&event);
	return(interval);

}
void MainGame::disable_button(ButtonFunc id) {
	int num = static_cast<int>(id);

	button_array[num].disabled = true;
	
}
void MainGame::enable_button(ButtonFunc id) {
	int num = static_cast<int>(id);

	button_array[num].disabled = false;
	

}
void MainGame::resume() {
	button_texture = SDL_CreateTextureFromSurface(renderer, button_surface[1]); 
	disable_button(ButtonFunc::NEXT);
	disable_button(ButtonFunc::CLEAR);
	disable_button(ButtonFunc::RESTART);
	disable_button(ButtonFunc::CHANGEPARTY);


	draw();
	_gameState = GameState::PLAY;
}
void MainGame::pause() {
	button_texture = SDL_CreateTextureFromSurface(renderer, button_surface[0]); 
	enable_button(ButtonFunc::NEXT);
	enable_button(ButtonFunc::CLEAR);
	enable_button(ButtonFunc::RESTART);
	enable_button(ButtonFunc::CHANGEPARTY);

	draw();
	_gameState = GameState::PAUSE;

}  

void MainGame::clear_graphs() {
	reset_point_arrays();
	SDL_SetRenderDrawColor(_graphR, Black.r, Black.g, Black.b, Black.a);
	SDL_RenderClear(_graphR);
	SDL_SetRenderDrawColor(_phasegraphR, Black.r, Black.g, Black.b, Black.a);
	SDL_RenderClear(_phasegraphR);
	old_phase_x = 0;
	old_phase_y = 0;
	SDL_RenderPresent(_graphR);
	SDL_RenderPresent(_phasegraphR);
}
void MainGame::clear() {
	cell_board.clear();
	draw();
	clear_graphs();
}
void MainGame::restart() {
	cell_board.restart();
	draw();
	clear_graphs();
}
void MainGame::change_party() {
	if (current_party == cellParty::PARTY1){
		current_party = cellParty::PARTY2;
	}
	else if (current_party == cellParty::PARTY2) {
		current_party = cellParty::PARTY1;
	}
	else {
		fatalError("current party is none!!!");
	}

	Button* change_party_button = &button_array[(int)ButtonFunc::CHANGEPARTY];
	int num = (int)current_party;
	std::string temp = "PARTY" + std::to_string(num);
	change_party_button->msg_surface = TTF_RenderText_Solid(title_font, temp.c_str(), { 0,0,0 });
	change_party_button->msg_texture = SDL_CreateTextureFromSurface(renderer, change_party_button->msg_surface); //now you can convert it into a texture
	draw();
}

void MainGame::set_renderer_color(SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

}

void MainGame::reset_point_arrays() {
	delete[] Party1;
	delete[] Party2;
	Party1 = new SDL_Point[_graphscreenWidth / graphx_step + 1];
	Party2 = new SDL_Point[_graphscreenWidth / graphx_step + 1];
	graphx = 0;
}

void MainGame::draw_timegraph() {
	int party1 = cell_board.count_party_one();
	int party2 = cell_board.count_party_two();
	int max = num_rect_horizontal * num_rect_vertical;
	
	int y1 = _graphscreenHeight - party1 * _graphscreenHeight / ((num_rect_horizontal * num_rect_vertical)/2);
	int y2 = _graphscreenHeight - party2 * _graphscreenHeight / ((num_rect_horizontal * num_rect_vertical)/2);
	SDL_SetRenderDrawColor(_graphR, 0, 0, 0, 255);
	SDL_RenderClear(_graphR);
	if (graphx < _graphscreenWidth) {
		SDL_SetRenderDrawColor(_graphR, Green.r, Green.g, Green.b, Green.a);
		SDL_Point temp1 = { graphx, y1 };
		SDL_Point temp2 = { graphx,y2 };
		Party1[graphx / graphx_step] = temp1;
		Party2[graphx / graphx_step] = temp2;
		SDL_RenderDrawLines(_graphR, Party1, graphx / graphx_step + 1);
		SDL_SetRenderDrawColor(_graphR, Cyan.r, Cyan.g, Cyan.b, Cyan.a);
		SDL_RenderDrawLines(_graphR, Party2, graphx / graphx_step + 1);

	}
	else {
		reset_point_arrays();
		SDL_Point temp1 = { graphx, y1 };
		SDL_Point temp2 = { graphx,y2 };
		Party1[graphx / graphx_step] = temp1;
		Party2[graphx / graphx_step] = temp2;
		SDL_RenderDrawLines(_graphR, Party1, graphx / graphx_step + 1);
		SDL_SetRenderDrawColor(_graphR, Cyan.r, Cyan.g, Cyan.b, Cyan.a);
		SDL_RenderDrawLines(_graphR, Party2, graphx / graphx_step + 1);
	}
	graphx = graphx + graphx_step;
	SDL_RenderPresent(_graphR);
	
	draw_phasegraph(party1, party2);
}

void MainGame::draw_phasegraph(int party1, int party2) {
	
	if (old_phase_x != 0 || old_phase_y != 0) {
		int x1 = old_phase_x * _graphscreenHeight / ((num_rect_horizontal * num_rect_vertical) / 2);
		int y1 = _graphscreenHeight - old_phase_y * _graphscreenHeight / ((num_rect_horizontal * num_rect_vertical) / 2);
		int x2 = party2 * _graphscreenHeight / ((num_rect_horizontal * num_rect_vertical) / 2);
		int y2 = _graphscreenHeight - party1 * _graphscreenHeight / ((num_rect_horizontal * num_rect_vertical) / 2);
		SDL_SetRenderDrawColor(_phasegraphR, Red.r, Red.g, Red.b, Red.a);
		SDL_RenderDrawLine(_phasegraphR, x1, y1, x2, y2);

		SDL_Rect r;
		r.x = x1;
		r.y = y1;
		r.w = 3;
		r.h = 3;
		SDL_RenderFillRect(_phasegraphR, &r);

		SDL_SetRenderDrawColor(_phasegraphR, White.r, White.g, White.b, White.a);
		r.x = x2;
		r.y = y2;
		r.w = 3;
		r.h = 3;
		SDL_RenderFillRect(_phasegraphR, &r);
		SDL_RenderPresent(_phasegraphR);
	}
	old_phase_x = party2;
	old_phase_y = party1;

	fout << party1 << "\t" << party2 << "\n";
}
