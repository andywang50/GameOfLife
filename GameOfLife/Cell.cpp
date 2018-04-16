#include "Cell.h"



cell::cell() {
	_currentState = cellState::Dead;
	_nextState = cellState::Dead;
	_party = cellParty::None;
	_nextparty = cellParty::None;

	_top = 0;
	_left = 0;
	_right = 0;
	_bot = 0;
	_highlight = false;

}
cell::~cell() {

	_top = 0;
	_left = 0;
	_right = 0;
	_bot = 0;
}
void cell::setNeighbors(int i, int j, int i_max, int j_max, cell ***cell_board ) {
	//if (i == 0) this->_top = cell_board[i_max][j];
	if (i == 0) this->_top = NULL;
	else this->_top = cell_board[i - 1][j];

	//if (i == i_max) this->_bot = cell_board[0][j];
	if (i == i_max) this->_bot = NULL;
	else this->_bot = cell_board[i + 1][j];

	//if (j == 0) this->_left = cell_board[i][j_max];
	if (j == 0) this->_left = NULL;
	else this->_left = cell_board[i][j-1];

	//if (j == j_max) this->_right = cell_board[i][0];
	if (j == j_max) this->_right = NULL;
	else this->_right = cell_board[i][j+1];
}
int cell::countAliveNeighbors() const {
	int count = 0;
	if (this->_top != NULL) {
		if (this->_top->_currentState == cellState::Alive) {
			count++;
		}
		if (this->_top->_left != NULL) {
			if (this->_top->_left->_currentState == cellState::Alive) count++;
		}
		if (this->_top->_right != NULL) {
			if (this->_top->_right->_currentState == cellState::Alive) count++;
		}
	}
	if (this->_bot != NULL) {
		if (this->_bot->_currentState == cellState::Alive) {
			count++;
		}
		if (this->_bot->_left != NULL) {
			if (this->_bot->_left->_currentState == cellState::Alive) count++;
		}
		if (this->_bot->_right != NULL) {
			if (this->_bot->_right->_currentState == cellState::Alive) count++;
		}
	}
	if (this->_left != NULL) {
		if (this->_left->_currentState == cellState::Alive) count++;
	}
	if (this->_right != NULL){
		if (this->_right->_currentState == cellState::Alive) count++;
	}
	
	
	return count;
}


	int cell::countPartyOne() const {
		int count = 0;
		if (this->_top != NULL) {
			if (this->_top->_party == cellParty::PARTY1) {
				count++;
			}
			if (this->_top->_left != NULL) {
				if (this->_top->_left->_party == cellParty::PARTY1) count++;
			}
			if (this->_top->_right != NULL) {
				if (this->_top->_right->_party == cellParty::PARTY1) count++;
			}
		}
		if (this->_bot != NULL) {
			if (this->_bot->_party == cellParty::PARTY1) {
				count++;
			}
			if (this->_bot->_left != NULL) {
				if (this->_bot->_left->_party == cellParty::PARTY1) count++;
			}
			if (this->_bot->_right != NULL) {
				if (this->_bot->_right->_party == cellParty::PARTY1) count++;
			}
		}
		if (this->_left != NULL) {
			if (this->_left->_party == cellParty::PARTY1) count++;
		}
		if (this->_right != NULL) {
			if (this->_right->_party == cellParty::PARTY1) count++;
		}


		return count;
}

SDL_Color cell::get_color(bool game_paused) const {
	
	
		if (_currentState == cellState::Dead) {
			if (game_paused == false || _highlight == false) {
				return Blue;
			}
			else { // highlighted & Dead
				return White;
			}


		}
		else { // Alive
			if (game_paused == false || _highlight == false) {
				if (_party == cellParty::PARTY1) {
					return Green;
				}
				else if (_party == cellParty::PARTY2) {
					return Cyan;
				}
			}
			else { // highlighted & Alive
				if (_party == cellParty::PARTY1) {
					return Yellow;
				}
				else if (_party == cellParty::PARTY2) {
					return Pink;
				}
			}
		}
		return Black;

}

cellState cell::get_currentState() const {
	return _currentState;
}
cellState cell::get_nextState() const {
	return _nextState;
}

cellParty cell::get_party() const {
	return _party;
}
cellParty cell::get_nextparty() const {
	return _nextparty;
}

void cell::set_nextState(cellState nextstate) {
	this->_nextState = nextstate;
}
void cell::set_nextParty(cellParty nextparty) {
	this -> _nextparty = nextparty;
}