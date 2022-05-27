#pragma once
#include<deque>
#include "cards.h"

class weissDeck {
	friend class weissPlayer;
	
public:
	weissDeck(int noOfCards=50, int noOfCx=8); //simple ctor with cards number and cx number only
	weissDeck(struct distribution); //ctor with specific distribution of cards
	weissDeck(const weissDeck& p1); //copy ctor


	void shuffle(void);
	unsigned getNoOfCards(void);

private:
	std::deque<card> mContent;


};