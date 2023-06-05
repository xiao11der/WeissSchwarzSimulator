#include <deque>
#include "cards.h"
#include "deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>
#include<random>

weissDeck::weissDeck(int noOfCards, int noOfCx) {
	if (noOfCx > 8) {
		throw std::invalid_argument("Too many Climaxes!");
	}

	card genericCx = card("climax"); //Create generic Cx card
	card genericChar = card("character");
	mContent = std::deque<card>(noOfCards, genericChar);
	
	for (int i = 0; i < noOfCx; i++) {
		mContent[i]=genericCx; //Insert generatic cx in the front of the deck
	}
	/*for (int i = 0; i < noOfCards - noOfCx; i++) {
		mDeck.push_front(card()); //Insert generic cards in the back of the deck
	}*/
	
}

void weissDeck::shuffle(void) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(mContent.begin(), mContent.end(), g);
}




unsigned weissDeck::getNoOfCards(void) {
	return mContent.size();
}

