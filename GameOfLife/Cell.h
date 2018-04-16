#ifndef _CELL_H
#define _CELL_H 1
#include <SDL/SDL.h>
#include "Color.h"

enum class cellState{Dead,Alive};
enum class cellParty{None, PARTY1, PARTY2};

class cell {
public:
	friend class MainGame;
	//friend class rule;
	friend class board;
	cell();
	~cell();
	void setNeighbors(int i, int j, int i_max, int j_max, cell ***cell_board);

	int countAliveNeighbors() const;
	int countPartyOne() const;
	int countPartyTwo() const;
	SDL_Color get_color(bool game_paused) const;
	
	cellState get_currentState() const;
	cellState get_nextState() const;
	cellParty get_party() const;
	cellParty get_nextparty() const;

	void set_nextState(cellState nextstate);
	void set_nextParty(cellParty nextparty);
private:
	cellState _currentState;
	cellState _nextState;

	cellParty _party;
	cellParty _nextparty;
	cell* _top;
	cell* _left;
	cell* _right;
	cell* _bot;

	bool _highlight;

	
	//int valueForTest;
};
#endif