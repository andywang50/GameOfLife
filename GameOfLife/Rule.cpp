#include "Rule.h"
#include <cstdlib>
rule::rule() {
	survive = NULL;
	born = NULL;
	party = NULL;
}

rule::~rule() {
	survive = NULL;
	born = NULL;
	party = NULL;
}

rule::rule(const rule& b) {
	survive = b.survive;
	born = b.born;
	party = b.party;
}
rule& rule::operator = (const rule& b) {
	if (this != &b) {
		survive = b.survive;
		born = b.born;
		party = b.party;
	}
	return *this;
}
cellState S23(cell* cell_ptr) {

	int aliveNeighbors = cell_ptr->countAliveNeighbors();

	if ((aliveNeighbors < 2) || (aliveNeighbors > 3)) { //nextState will be dead
		return cellState::Dead;
	}
	else {  //survive
		return cellState::Alive;

	}
}

cellState B3(cell* cell_ptr) {

	int aliveNeighbors = cell_ptr->countAliveNeighbors();

	if (aliveNeighbors == 3) {

		return cellState::Alive;

	}
	else {

		return cellState::Dead;

	}
}

cellParty two_party(cell* cell_ptr) {



	if (cell_ptr->get_nextState() == cellState::Dead) {
		//cell_ptr->set_nextParty(cellParty::None);
		return cellParty::None;
	}
	else {
		if (cell_ptr->get_currentState() == cellState::Dead) {

			int aliveNeighbors = cell_ptr->countAliveNeighbors();
			int num_partyone = cell_ptr->countPartyOne();
			int num_partytwo = aliveNeighbors - num_partyone;

			if (num_partyone > num_partytwo) {
				//cell_ptr->set_nextParty(cellParty::PARTY1);
				return cellParty::PARTY1;
			}
			else if (num_partyone < num_partytwo) {
				//cell_ptr->set_nextParty(cellParty::PARTY2);
				return cellParty::PARTY2;
			}
			else {
				int temp2 = rand() % 100 + 1;
				if (temp2 <= 50) {
					//cell_ptr->set_nextParty(cellParty::PARTY1);
					return cellParty::PARTY1;

				}
				else {
					//cell_ptr->set_nextParty(cellParty::PARTY2);
					return cellParty::PARTY2;

				}
			}
		}
		else {
			return cell_ptr->get_party();
		}
	}
}

cellState S1357(cell* cell_ptr) {
	int aliveNeighbors = cell_ptr->countAliveNeighbors();

	if (aliveNeighbors % 2 == 0) { //nextState will be dead
		return cellState::Dead;
	}
	else {  //survive
		return cellState::Alive;

	}
}

cellState B1357(cell* cell_ptr) {
	int aliveNeighbors = cell_ptr->countAliveNeighbors();

	if (aliveNeighbors % 2 == 1) {
		return cellState::Alive;

	}
	else {
		return cellState::Dead;

	}
}

cellState B_Anton(cell* cell_ptr) {
	int aliveNeighbors = cell_ptr->countAliveNeighbors();
	int party1 = cell_ptr->countPartyOne();
	int party2 = aliveNeighbors - party1;

	if ((party1 == 3) && (party2 != 3)) {
		cell_ptr->set_nextParty(cellParty::PARTY1);
		return cellState::Alive;
	}
	else if ((party1 != 3) && (party2 == 3)) {
		cell_ptr->set_nextParty(cellParty::PARTY2);

		return cellState::Alive;

	}
	else { cell_ptr->set_nextParty(cellParty::None); return cellState::Dead; }
}

cellState S_Anton(cell* cell_ptr) {
	cellParty party = cell_ptr->get_party();
	int aliveNeighbors = cell_ptr->countAliveNeighbors();
	int party1 = cell_ptr->countPartyOne();
	int party2 = aliveNeighbors - party1;
	if (party == cellParty::PARTY1) {
		if (party1 >= 2 && party1 <= 3) { cell_ptr->set_nextParty(party); return cellState::Alive; }
		else {
			cell_ptr->set_nextParty(cellParty::None);
			return cellState::Dead; 
		}

	}
	else {
		if (party2 >= 2 && party2 <= 3) { cell_ptr->set_nextParty(party); return cellState::Alive; 		}
		else {
			cell_ptr->set_nextParty(cellParty::None);
			return cellState::Dead; }
	}
}

cellParty party_Anton(cell* cell_ptr) {
	
	return cell_ptr->get_nextparty();
}

cellState S_pp(cell* cell_ptr) {
	// Assume party1 = predator, party2 = prey;
	int aliveNeighbors = cell_ptr->countAliveNeighbors();
	int party1 = cell_ptr->countPartyOne();
	int party2 = aliveNeighbors - party1;
	if (cell_ptr->get_party() == cellParty::PARTY1) { // A predator
		if (party2 < 2) {
			cell_ptr->set_nextParty(cellParty::None);
			return cellState::Dead;
		}
		else {
			cell_ptr->set_nextParty(cell_ptr->get_party());
			return cellState::Alive;
		}
	}
	else { // A prey
		if ((party1 >= 1) || (party2<2)) {
			cell_ptr->set_nextParty(cellParty::None);
			return cellState::Dead;
		}
		else {
			cell_ptr->set_nextParty(cell_ptr->get_party());
			return cellState::Alive;
		}
	}
}

cellState B_pp(cell* cell_ptr) {
	// Assume party1 = predator, party2 = prey;
	int aliveNeighbors = cell_ptr->countAliveNeighbors();
	int party1 = cell_ptr->countPartyOne();
	int party2 = aliveNeighbors - party1;
	if ((party1 >= 2) && (party2 >= 1)) { // a predator is born
		cell_ptr->set_nextParty(cellParty::PARTY1);
		return cellState::Alive;
	}
	else if ((party2 >= 2) && (party1 <= 1)) { // a prey is born
		cell_ptr->set_nextParty(cellParty::PARTY2);
		return cellState::Alive;
	}
	else {
		cell_ptr->set_nextParty(cell_ptr->get_party());

		return cellState::Dead;
	}
}

cellParty party_pp(cell* cell_ptr) {
	// Assume party1 = predator, party2 = prey;
	
	return cell_ptr->get_nextparty();
}
// below are the life rules involving probability parameters

cellState S_ppp(cell* cell_ptr) {
	// Assume party1 = predator, party2 = prey;
	int aliveNeighbors = cell_ptr->countAliveNeighbors();
	int party1 = cell_ptr->countPartyOne();
	int party2 = aliveNeighbors - party1;
	if (cell_ptr->get_party() == cellParty::PARTY1) { // A predator
		int predator_natural_death = rand() % 100 + 1;
		if (predator_natural_death <= 0) {
			cell_ptr->set_nextParty(cellParty::None);
			return cellState::Dead;
		}
		else {
			int predator_enough_food = rand() % 100 + 1;
			//double prey_predator_ratio = party2 / (party1 + 1);
			if (predator_enough_food <= party2 * 100) { // enough food
				cell_ptr->set_nextParty(cell_ptr->get_party());
				return cellState::Alive;
			}
			else {
				cell_ptr->set_nextParty(cellParty::None);
				return cellState::Dead;
			}
		}
	}
	else { // A prey
		if (party2 < 2 || party2 >= 6) { //lonely or overcrowded
			cell_ptr->set_nextParty(cellParty::None);
			return cellState::Dead;
		}
		else {
			int being_chased = rand() % 100 + 1;
			//double predator_prey_ratio = party1 / (party2 + 1);
			if (being_chased <= party1 * 100) { //being chased by a predator but have possibility to escape
				double escape_prob = 0.2;
				int escape = rand() % 100 + 1;
				if (escape <= escape_prob * 100) { //escape and survive
					cell_ptr->set_nextParty(cell_ptr->get_party());
					return cellState::Alive;
				}
				else {
					cell_ptr->set_nextParty(cellParty::None);
					return cellState::Dead;
				}

			}
			else { //not chased
				cell_ptr->set_nextParty(cell_ptr->get_party());
				return cellState::Alive;

			}
		}
	}
}

cellState B_ppp(cell* cell_ptr) {
	// Assume party1 = predator, party2 = prey;
	int aliveNeighbors = cell_ptr->countAliveNeighbors();
	int party1 = cell_ptr->countPartyOne();
	int party2 = aliveNeighbors - party1;
	if (party1 >= 2) {
		double mate_prob = 0;
		switch (party1) {
			case 2: {
				mate_prob = 0.9;
				break;
			}
			case 3: {
				mate_prob = 1;
				break;
			}
			case 4: {
				mate_prob = 1;
				break;
			}
			default: {
				mate_prob = 1;
				break;
			}
		}
		double food_factor = 0;
		// (party2 >= 3) food_factor = 1;
		food_factor = party2 / 2;
		int reproduction_predator_occur = rand() % 100 + 1;
		if (reproduction_predator_occur <= mate_prob*food_factor * 100) {
			cell_ptr->set_nextParty(cellParty::PARTY1);
			return cellState::Alive;
		}
		else {
			cell_ptr->set_nextParty(cellParty::None);
			return cellState::Dead;
		}
	}
	else { // a reproduction of predator will not occur, check the conditions of reproduction of preys
		double mate_prob = 0;
		switch (party2) {
			case 0: {
				mate_prob = 0;
				break;
			}
			case 1: {
				mate_prob = 0;
				break;
			}
			case 2: {
				mate_prob = 0.5;
				break;
			}
			default: {
				mate_prob = 1;
				break;
			}
		}
		double safe_factor = 0;
		if (party1 >= 2) safe_factor = 0.2;
		else if (party1 == 1) safe_factor = 0.4;
		else safe_factor = 1;
		int reproduction_prey_occur = rand() % 100 + 1;
		if (reproduction_prey_occur <= mate_prob*safe_factor * 100) {
			cell_ptr->set_nextParty(cellParty::PARTY2);
			return cellState::Alive;
		}
		else {
			cell_ptr->set_nextParty(cellParty::None);
			return cellState::Dead;
		}
	}
}