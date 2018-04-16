#include "board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>;

board::board() {
	cell_board = NULL;


}
board::board(int ymax, int xmax):x_max(xmax), y_max(ymax) {
	srand(time(0));
	cell_board = new cell**[ymax + 1];
	for (int i = 0; i <= ymax; i++) {
		cell_board[i] = new cell*[xmax + 1];
		for (int j = 0; j <= xmax; j++) {
			cell_board[i][j] = new cell;
			
		}

	}
	restart();
	

	using_rule.born = &B_pp; // B_3, B_1357, B_Anton
	using_rule.survive = &S_pp; //S_23, S1357, S_Anton
	using_rule.party = &party_pp; // party_pp, two_party

}
board::board(const board& b) {
	x_max = b.x_max;
	y_max = b.y_max;
	using_rule = b.using_rule;


	cell_board = new cell**[y_max + 1];
	for (int i = 0; i <= y_max; i++) {
		cell_board[i] = new cell*[x_max + 1];
		for (int j = 0; j <= x_max; j++) {
			cell_board[i][j] = new cell;
			cell_board[i][j]->_currentState = b.cell_board[i][j]->_currentState;
			cell_board[i][j]->_party = b.cell_board[i][j]->_party;

		}

	}
}
board& board::operator = (const board& b) {
	if (this != &b) {
		x_max = b.x_max;
		y_max = b.y_max;
		using_rule = b.using_rule;

		cell_board = new cell**[y_max + 1];
		for (int i = 0; i <= y_max; i++) {
			cell_board[i] = new cell*[x_max + 1];
			for (int j = 0; j <= x_max; j++) {
				cell_board[i][j] = new cell;
				cell_board[i][j]->_currentState = b.cell_board[i][j]->_currentState;
				cell_board[i][j]->_party = b.cell_board[i][j]->_party;

			}

		}
	}
	return *this;
}
board::~board() {
	if (cell_board != NULL)
	{
		for (int i = 0; i <= y_max; i++) {
			for (int j = 0; j <= x_max; j++) {
				delete cell_board[i][j];
				cell_board[i][j] = NULL;
			}
			delete[] cell_board[i];
			cell_board[i] = NULL;
		}
		delete[] cell_board;
		cell_board = NULL;
	}
}
void board::setNeighbors() {

	for (int i = 0; i <= y_max; i++) {
		for (int j = 0; j <= x_max; j++) {


			cell_board[i][j]->setNeighbors(i, j, y_max, x_max , cell_board);
		}
	}
}
cell** board::operator[](int y_index) {

	return cell_board[y_index];
}
void board::highlight(int y_coord, int x_coord) {
	
	if ((x_coord >= 0) && (y_coord >= 0) && (x_coord <= x_max) && (y_coord <= y_max)) {
		if (mouse_outof_bound == false) {
			if ((current_highlight_x != x_coord) || (current_highlight_y != y_coord)) {
				cell* cell_ptr = cell_board[current_highlight_y][current_highlight_x];
				cell_ptr->_highlight = false;
				cell_ptr = cell_board[y_coord][x_coord];
				current_highlight_x = x_coord;
				current_highlight_y = y_coord;
				cell_ptr->_highlight = true;
			}
		}
		else {
			mouse_outof_bound = false;
			cell* cell_ptr = cell_board[y_coord][x_coord];
			current_highlight_x = x_coord;
			current_highlight_y = y_coord;
			cell_ptr->_highlight = true;
		}

	}
	else {
		if (mouse_outof_bound == false) {
			mouse_outof_bound = true;
			cell* cell_ptr = cell_board[current_highlight_y][current_highlight_x];
			cell_ptr->_highlight = false;
		
		}
	}

}
void board::changeCellState(int y_coord, int x_coord,cellParty current_party) {
	
		cell* cell_ptr = cell_board[y_coord][x_coord];
		if (cell_ptr->_currentState == cellState::Alive) {
			cell_ptr->_currentState = cellState::Dead;
			cell_ptr->_party = cellParty::None;

		}
		else {
			cell_ptr->_currentState = cellState::Alive;
			cell_ptr->_party = current_party;
		}
	
}
void board::calc_nextStage() {
	for (int i = 0; i <= y_max; i++) {
		for (int j = 0; j <= x_max; j++) {
			cell* cell_ptr = cell_board[i][j];

			/*
			int aliveNeighbors = cell_ptr->countAliveNeighbors();
			if (cell_ptr->_currentState == cellState::Alive) {
				if ((aliveNeighbors < underPopulation) || (aliveNeighbors > overPopulation)) { //nextState will be dead
					cell_ptr->_nextState = cellState::Dead;
					cell_ptr->_nextparty = cellParty::None;

				}
				else {  //survive
					cell_ptr->_nextState = cellState::Alive;
					cell_ptr->_nextparty = cell_ptr->_party;

				}

			}
			else { //dead
				if ((aliveNeighbors == reproduction) || (aliveNeighbors == highlife)) { //reproduced
					cell_ptr->_nextState = cellState::Alive;
					int num_partyone = cell_ptr->countPartyOne();
					int num_partytwo = aliveNeighbors - num_partyone;
					if (num_partyone > num_partytwo) cell_ptr->_nextparty = cellParty::PARTY1;
					else if (num_partyone < num_partytwo) cell_ptr->_nextparty = cellParty::PARTY2;
					else {
						int temp2 = rand() % 100 + 1;
						if (temp2 <= 50) {
							cell_ptr->_nextparty = cellParty::PARTY1;
						}
						else {
							cell_ptr->_nextparty = cellParty::PARTY2;
						}
					}
					//redraw(i, j, cell_ptr->_nextState);
				}
				else {  // no reproduction happens
					
					cell_ptr->_nextState = cellState::Dead;
					cell_ptr->_nextparty = cell_ptr->_party;

				}
			}
			*/
			if (cell_ptr->_currentState == cellState::Alive) {
				cell_ptr->_nextState = using_rule.survive(cell_ptr);
			}
			else {
				cell_ptr->_nextState = using_rule.born(cell_ptr);

			}

			cell_ptr->_nextparty = using_rule.party(cell_ptr);
		
		}

	}
}
void board::update_nextStage() {
	for (int i = 0; i <= y_max; i++) {
		for (int j = 0; j <= x_max; j++) {
			cell* cell_ptr = cell_board[i][j];
			
			cell_ptr->_party = cell_ptr->_nextparty;


			cell_ptr->_currentState = cell_ptr->_nextState;

			cell_ptr->_nextState = cellState::Alive;
			
		}
		
	}

}
void board::clear() {
	for (int i = 0; i <= y_max; i++) {
		for (int j = 0; j <= x_max; j++) {
			cell* cell_ptr = cell_board[i][j];
			cell_ptr->_currentState = cellState::Dead;
			cell_ptr->_nextState = cellState::Dead;
			cell_ptr->_nextparty = cellParty::None;
			cell_ptr->_party = cellParty::None;
		}
	}
}
void board::restart() {
	for (int i = 0; i <= y_max; i++) {
		for (int j = 0; j <= x_max; j++) {
			cell* cell_ptr = cell_board[i][j];
			cell_ptr->_currentState = cellState::Dead;
			cell_ptr->_nextparty = cellParty::None;
			int temp = rand() % 100 + 1;
			if (temp <= 10) {
				cell_ptr->_currentState = cellState::Alive;
				cell_ptr->_nextState = cellState::Dead;
				int temp2 = rand() % 100 + 1;
				if (temp2 <= 30) {
					cell_ptr->_party = cellParty::PARTY1;
				}
				else {
					cell_ptr->_party = cellParty::PARTY2;
				}

			}
			else {
				cell_ptr->_currentState = cellState::Dead;
				cell_ptr->_nextState = cellState::Dead;
				cell_ptr->_party = cellParty::None;

			}
		}
	}
}

int board::count_party_one() {
	int count = 0;
	for (int i = 0; i <= y_max; i++) {
		for (int j = 0; j <= x_max; j++) {
			cell* cell_ptr = cell_board[i][j];
			if (cell_ptr->get_party() == cellParty::PARTY1) count++;
		}
	}
	return count;
}
int board::count_party_two() {
	int count = 0;
	for (int i = 0; i <= y_max; i++) {
		for (int j = 0; j <= x_max; j++) {
			cell* cell_ptr = cell_board[i][j];
			if (cell_ptr->get_party() == cellParty::PARTY2) count++;

		}
	}
	return count;
}
