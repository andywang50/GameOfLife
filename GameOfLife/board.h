#ifndef _BOARD_H
#define _BOARD_H 1
#include "Cell.h"
#include "Rule.h";
class board {
public:
	friend class Maingame;
	friend class cell;
	board();
	~board();

	
	board(const board& b);
	
	board& operator = (const board& b);
	
	board(int ymax, int xmax);
	cell** operator[](int y_index);

	void setNeighbors();
	void highlight(int y_coord, int x_coord);
	void changeCellState(int y_coord, int x_coord, cellParty current_party);
	void calc_nextStage();
	void update_nextStage();
	void clear();
	void restart();

	rule using_rule;
	
	int count_party_one();
	int count_party_two();

private:
	cell*** cell_board;
	int x_max;
	int y_max;

	int current_highlight_x;
	int current_highlight_y;

	bool mouse_outof_bound;
	
};

#endif