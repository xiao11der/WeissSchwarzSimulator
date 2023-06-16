#include "weissPlayer.h"
#include "deck.h"
#include<iostream>



weissPlayer::weissPlayer(weissDeck deckState) { //Ctor with deck only
	mPlayerDeck = deckState;

	//zero-initialize all other zones
	mPlayerClock = weissDeck(0, 0);
	mPlayerStock = weissDeck(0, 0);
	mPlayerLevel = weissDeck(0, 0);
	mPlayerWaitingRoom = weissDeck(0, 0);
	mPlayerMemory = weissDeck(0, 0);


	mPlayerResolution = weissDeck(0, 0); 
	mAddSoul = 0; //No additional souls until trigger check

}

weissPlayer::weissPlayer(weissDeck deckState, weissDeck waitingRoomState) { //Ctor with deck and waitingroom
	mPlayerDeck = deckState;
	mPlayerWaitingRoom = waitingRoomState;


	//zero-initialize all other zones
	mPlayerClock = weissDeck(0, 0);
	mPlayerStock = weissDeck(0, 0);
	mPlayerLevel = weissDeck(0, 0);
	mPlayerMemory = weissDeck(0, 0);


	mPlayerResolution = weissDeck(0, 0);
	mAddSoul = 0; //No additional souls until trigger check

}


weissPlayer::weissPlayer(weissDeck deckState, weissDeck waitingRoomState, weissDeck clockState, weissDeck levelState, weissDeck stockState, weissDeck memoryState) //Generic ctor
{
	//Linker error if done in initializer list?
	mPlayerDeck = deckState;
	mPlayerClock = clockState;
	mPlayerStock = stockState;
	mPlayerLevel = levelState;
	mPlayerWaitingRoom = waitingRoomState;
	mPlayerMemory=memoryState;
	
	
	mPlayerResolution = weissDeck(0, 0); //zero-initialize resolution zone 
	mAddSoul = 0; //No additional souls until trigger check
}

weissPlayer::weissPlayer(const weissPlayer& p1)

{
	mPlayerDeck = p1.mPlayerDeck;
	mPlayerClock = p1.mPlayerClock;
	mPlayerStock = p1.mPlayerStock;
	mPlayerWaitingRoom = p1.mPlayerWaitingRoom;
	mPlayerMemory = p1.mPlayerMemory;
	mPlayerLevel = p1.mPlayerLevel;
	
	mPlayerResolution = p1.mPlayerResolution; //zero-initialize resolution zone
	mAddSoul = 0; //No additional souls until trigger check
} 

void weissPlayer::shuffleDeck(void) {
	mPlayerDeck.shuffle();
}


deckReport weissPlayer::createReportXCards(int x, peekPos peekSide, postReport postReportAction) {

	deckReport outStruct;

	if (x > 50) {
		throw std::runtime_error("You cannot look at more than 50 cards");
	}

	auto determineType = [](card inputCard) {
		switch (inputCard.getType()) {
		case CHARACTER:
			return std::string("character");
		case CLIMAX:
			return std::string("climax");
		case EVENT:
			return std::string("event");
		default:
			throw std::runtime_error("Error while parsing card type trying to generate deck report, is the input validation active?");
		}
	};

	auto determineColor = [](card inputCard) {
		switch (inputCard.getColor()) {
		case BLUE:
			return std::string("blue");
		case RED:
			return std::string("red");
		case GREEN:
			return std::string("green");
		case YELLOW:
			return std::string("yellow");
		default:
			throw std::runtime_error("Error while parsing card color trying to generate deck report, is the input validation active?");
		}
	};

	switch (peekSide) {
	case TOP:
		for (int i = 0; i < x; ++i) {
			card topCard = mPlayerDeck.mContent.front();
			if (topCard.getType() == CLIMAX) {
				outStruct.noOfCx++;
			}

			outStruct.levelDist[topCard.getLevel()]++;
			outStruct.triggerDist[topCard.getTrigger()]++;
			outStruct.colorDist[topCard.getColor()]++;
			outStruct.raw.append(std::format("|t:{} c:{} l:{} t:{} s:{}", determineType(topCard), determineColor(topCard), topCard.getLevel(), topCard.getTrigger(), topCard.getSoul()));
			mPlayerResolution.mContent.push_back(topCard);
			mPlayerDeck.mContent.pop_front();
		}
		break;
	case BOTTOM:
		for (int i = 0; i < x; ++i) {
			card botCard = mPlayerDeck.mContent.back();
			if (botCard.getType() == CLIMAX) {
				outStruct.noOfCx++;
			}

			outStruct.levelDist[botCard.getLevel()]++;
			outStruct.triggerDist[botCard.getTrigger()]++;
			outStruct.colorDist[botCard.getColor()]++;
			outStruct.raw.append(std::format("|t:{} c:{} l:{} t:{} s:{}", determineType(botCard), determineColor(botCard), botCard.getLevel(), botCard.getTrigger(), botCard.getSoul()));
			mPlayerResolution.mContent.push_back(botCard);
			mPlayerDeck.mContent.pop_back();
		}
		break;
	default:
		throw std::runtime_error("You must look from top or bottom of deck when creating deck report, keywords are TOP and BOTTOM");
	}

	switch (postReportAction) {
	case WR:
		combine(mPlayerResolution, mPlayerWaitingRoom);
		break;
	case TOPDECK: //back to top of deck
		for (int i=0;i<x;++i){
			mPlayerDeck.mContent.push_front(mPlayerResolution.mContent.back());
			mPlayerResolution.mContent.pop_back();
		}
		break;
	case BOTDECK: //goes to bottom deck in the reverse order it came in
		for (int i = 0; i < x; ++i) {
			mPlayerDeck.mContent.push_back(mPlayerResolution.mContent.back());
			mPlayerResolution.mContent.pop_back();
		}
		break;
	case DECKSHUFFLE:
		combine(mPlayerResolution, mPlayerDeck);
		mPlayerDeck.shuffle();
		break;
	default:
		throw std::runtime_error("Wrong mode for manipulating X cards, it must be WR, TOPDECK, BOTDECK, or DECKSHUFFLE");
	}


	return outStruct;
}

void weissPlayer::triggerCheck(void) {
	
	//Check trigger and add soul
	card trigger = mPlayerDeck.mContent.front();
	mAddSoul += trigger.getTrigger();

	mPlayerStock.mContent.push_front(trigger);
	mPlayerDeck.mContent.pop_front();

	if (mPlayerDeck.getNoOfCards() == 0) {
		combine(mPlayerWaitingRoom, mPlayerDeck);
		mPlayerDeck.shuffle();
		mQueueRefreshDamage = true;
	}

	if (mQueueRefreshDamage) {

		takeRefreshDamage();

	}


}

void weissPlayer::combine(weissDeck& deckSource, weissDeck& deckDest) {

	for (auto card : deckSource.mContent) {
		deckDest.mContent.push_back(card);
	}
	deckSource.mContent.clear();

}


bool weissPlayer::takeDamage(int amount) { //Overall damage calculation for cancellable damage
	
	if (burnDeck(amount,true)) { // if damage sticks
		combine(mPlayerResolution, mPlayerClock);
		while(mPlayerClock.getNoOfCards() >= 7) { //Level up
			weissPlayer::levelUp();
		}

		if (mQueueRefreshDamage) {//Take refresh damage if necessary
			weissPlayer::takeRefreshDamage();
		}

		return true; //return damage sticks
	}
	else { // if damage cancels
		combine(mPlayerResolution, mPlayerWaitingRoom);
		if (mQueueRefreshDamage) {//Take refresh damage if necessary
			weissPlayer::takeRefreshDamage();
		}

		return false; //return damage cancel
	}

}

void weissPlayer::takeUncanDamage(int amount) { //Overall damage calculation for uncancellable damage
	burnDeck(amount, false);
	combine(mPlayerResolution, mPlayerClock);
	while (mPlayerClock.getNoOfCards() >= 7) { //Level up
		weissPlayer::levelUp();
	}

	if (mQueueRefreshDamage) {//Take refresh damage if necessary
		weissPlayer::takeRefreshDamage();
	}

}

void weissPlayer::shuffleBackNonCX(int amount) {

	int cardsPutBack = 0;
	card topWaitingRoom;
	while (!mPlayerWaitingRoom.mContent.empty()) {
		topWaitingRoom = mPlayerWaitingRoom.mContent.front();

		if (topWaitingRoom.getType() != CLIMAX) {
			mPlayerDeck.mContent.push_front(topWaitingRoom);
			cardsPutBack++;
		}
		else {
			mPlayerResolution.mContent.push_back(topWaitingRoom);
		}

		mPlayerWaitingRoom.mContent.pop_front();
		if (cardsPutBack == amount) {
			break;
		}
	}
	combine(mPlayerResolution, mPlayerWaitingRoom);
	mPlayerDeck.shuffle();

}

void weissPlayer::stockWash(stockWashMode mode) {
	int noOfStock = mPlayerStock.getNoOfCards();
	switch (mode) {
	case TOWR:
		combine(mPlayerStock, mPlayerWaitingRoom);
		blindStock(noOfStock);
		break;
	case TODECK:
		combine(mPlayerStock, mPlayerDeck);
		mPlayerDeck.shuffle();
		blindStock(noOfStock);
		break;
	default:
		throw std::runtime_error("Wrong stock wash mode detected");
	}
}

void weissPlayer::blindStock(int amount) {
	for (int i = 0; i < amount; ++i) {
		mPlayerStock.mContent.push_back(mPlayerDeck.mContent.front());
		mPlayerDeck.mContent.pop_front();
	}
}

bool weissPlayer::burnDeck(int damage, bool canBeCanceled) { //Subfunction to take x cancellable damage from top deck and refresh as necessary
	card topCard;

	for (int i = 0; i < damage; i++) {
		topCard = mPlayerDeck.mContent.front();
		mPlayerResolution.mContent.push_back(topCard); //Push to resolution zone
		mPlayerDeck.mContent.pop_front();

		if (mPlayerDeck.getNoOfCards() == 0) { //Refresh if deck runs out, queue refresh damage if needed
			combine(mPlayerWaitingRoom, mPlayerDeck);
			mPlayerDeck.shuffle();
			mQueueRefreshDamage=true;
		}

		
		if (canBeCanceled) {
			if (topCard.getType() == CLIMAX) {
				return false; //damage canacled
			}
		}
	}
	return true;
}

void weissPlayer::levelUp(void) {

	for (auto i = mPlayerClock.mContent.begin(); i <= mPlayerClock.mContent.begin()+6; ++i) { //Iterate the first 7 cards of the clock, find card to level up, level up the first non-cx
		card topClock = *i; //iterate through deque
		if (topClock.getType() != CLIMAX) {
			mPlayerLevel.mContent.push_back(topClock);
			mPlayerClock.mContent.pop_front();
			//std::cout << "found non-cx" << std::endl;
			break;
		}

		if (i == mPlayerClock.mContent.begin() + 5) { //If the entire clock is cx, just level a cx
			mPlayerLevel.mContent.push_back(topClock);
			mPlayerClock.mContent.pop_front();
			break;
			//std::cout << "all clock is cx" << std::endl;
		}

		//std::cout << "card cx, moving on to the next" << std::endl;
	}
	 //level first card of clock
	
	int overFlow = mPlayerClock.getNoOfCards() - 6;

	for (int i = 0; i < overFlow; ++i) { //-6 since we leveled up one card, only 6 is supposed to go to waiting room, dump the overflow to resolution first
		mPlayerResolution.mContent.push_front(mPlayerClock.mContent.back());
		mPlayerClock.mContent.pop_back();
	}



	combine(mPlayerClock, mPlayerWaitingRoom); //Dump clock to waiting room
	combine(mPlayerResolution, mPlayerClock); //Dump overflow to clock again
}

void weissPlayer::takeRefreshDamage(void) {

	mPlayerClock.mContent.push_back(mPlayerDeck.mContent.front()); //put top card of deck to clock
	mPlayerDeck.mContent.pop_front();
	mQueueRefreshDamage = false;

	while (mPlayerClock.getNoOfCards() >= 7) { //Level up if needed
		weissPlayer::levelUp();
	}

}

void weissPlayer::endOfAttack(void) {

	//Clean up function at end of each swing

	//Clear additional soul
	mAddSoul = 0;
}

weissDeck& weissPlayer::getClock(void) {
	return mPlayerClock;
}

weissDeck& weissPlayer::getLevel(void) {
	return mPlayerLevel;

}

int weissPlayer::getCurrentSoul(void) {
	return mAddSoul;
}

