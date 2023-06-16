#pragma once
#include<deque>
#include "cards.h"


//Helper functions that help with effects


class weissDeck {
	friend class weissPlayer;
	
public:
	weissDeck(int noOfCards=50, int noOfCx=8); //simple ctor with cards number and cx number only
	weissDeck(std::string file); //import deck from text file
	weissDeck(const weissDeck& p1); //copy ctor



	


	void shuffle(void);
	unsigned getNoOfCards(void);


private:
	std::deque<card> mContent;


};