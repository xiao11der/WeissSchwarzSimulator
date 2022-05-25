#include <deque>
#include "cards.h"
#include "deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>

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
	std::random_shuffle(mContent.begin(), mContent.end());
}

bool weissDeck::takeDamage(int damage, weissDeck& resolution) {
	card topCard;

	for (int i = 0; i < damage; i++) {
		topCard = mContent.front();
		resolution.mContent.push_back(topCard); //Push to resolution zone
		mContent.pop_front();
		
		if (topCard.getType() == "climax") {
			return false; //damage canacled
		}
	}
	return true;
}


unsigned weissDeck::getNoOfCards(void) {
	return mContent.size();
}

