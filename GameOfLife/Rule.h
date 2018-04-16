#ifndef RULE_H
#define RULE_H 1

#include "Cell.h"

cellState S23(cell*);
cellState S1357(cell*);
cellState B3(cell*);
cellState B1357(cell*);
cellParty two_party(cell*);


cellState S_Anton(cell*);
cellState B_Anton(cell*);
cellParty party_Anton(cell*);

cellState S_pp(cell*);
cellState B_pp(cell*);
cellParty party_pp(cell*);

cellState S_ppp(cell*);
cellState B_ppp(cell*);
class rule {
public:

	friend class board;
	rule();
	~rule();


	rule(const rule& b);

	rule& operator = (const rule& b);
private:
	

	cellState (*survive)(cell*);
	cellState (*born)(cell*);
	cellParty (*party)(cell*);

};



#endif