#include <deque>
#include "cards.h"
#include "deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>
#include<random>
#include<utility>
#include<fstream>
#include "common.h"

weissDeck::weissDeck(int noOfCards, int noOfCx) {
	if (noOfCx > 8) {
		throw std::invalid_argument("Too many Climaxes!");
	}

	card genericCx = card(CLIMAX); //Create generic Cx card
	card genericChar = card(CHARACTER);
	mContent = std::deque<card>(noOfCards, genericChar);
	
	for (int i = 0; i < noOfCx; i++) {
		mContent[i]=genericCx; //Insert generatic cx in the front of the deck
	}
	/*for (int i = 0; i < noOfCards - noOfCx; i++) {
		mDeck.push_front(card()); //Insert generic cards in the back of the deck
	}*/
	
}

weissDeck::weissDeck(std::string file) {
	std::string line;
	std::ifstream fileStream(file);
	//TO-DO: USE REGEX and implement error checks to give user warnings
	while (std::getline(fileStream, line)) { //Obtain cards, using format [Type: C=Character, E=Event, X=Climax][Color: B=Blue, Y=Yellow, R=Red, G=Green][Level (int)][Trigger(int)][Soul(int)]
		// "//" is comment
		if (line.rfind("//", 0) == 0 || line == "") {
			continue;
		}


		cardType type = CHARACTER;
		cardColor color = BLUE;
		int level = -1;
		int soul = -1;
		int trigger = -1;
		trim(line);
		
		switch (line[0]) {
		case 'C':
			type = CHARACTER;
			break;
		case 'E':
			type = EVENT;
			break;
		case 'X':
			type = CLIMAX;
			break;
		default:
			throw std::invalid_argument("Wrong card type during readin, must be C=Character, E=Event, X=Climax");
		}

		switch (line[1]) {
		case 'B':
			color = BLUE;
			break;
		case 'Y':
			color = YELLOW;
			break;
		case 'G':
			color = GREEN;
			break;
		case 'R':
			color = RED;
			break;
		default:
			throw std::invalid_argument("Wrong color during read in, must be B=Blue, Y=Yellow, R=Red, G=Green");
		}

		level = line[2]-'0';
		trigger = line[3] - '0';
		soul = line[4] - '0';

		mContent.push_back(card(level, soul, trigger, color, type));

	}
}

void weissDeck::shuffle(void) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(mContent.begin(), mContent.end(), g);
}



unsigned weissDeck::getNoOfCards(void) {
	return mContent.size();
}

