#include "weissPlayer.h"
#include "deck.h"



weissPlayer::weissPlayer(weissDeck deckState) {
	mPlayerDeck = deckState;

	//zero-initialize all other zones
	mPlayerClock = weissDeck(0, 0);
	mPlayerStock = weissDeck(0, 0);
	mPlayerLevel = weissDeck(0, 0);
	mPlayerWaitingRoom = weissDeck(0, 0);
	mPlayerMemory = weissDeck(0, 0);


	mPlayerResolution = weissDeck(0, 0); 

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
}

weissPlayer::weissPlayer(weissPlayer& p1)

{
	mPlayerDeck = p1.mPlayerDeck;
	mPlayerClock = p1.mPlayerClock;
	mPlayerStock = p1.mPlayerStock;
	mPlayerWaitingRoom = p1.mPlayerWaitingRoom;
	mPlayerMemory = p1.mPlayerMemory;
	mPlayerLevel = p1.mPlayerLevel;
	
	mPlayerResolution = p1.mPlayerResolution; //zero-initialize resolution zone
} 

void weissPlayer::shuffleDeck(void) {
	mPlayerDeck.shuffle();
}
void weissPlayer::combine(weissDeck& deckSource, weissDeck& deckDest) {

	for (auto card : deckSource.mContent) {
		deckDest.mContent.push_back(card);
	}
	deckSource.mContent.clear();

}


void weissPlayer::takeDamage(int amount) {
	
	if (mPlayerDeck.takeDamage(amount, mPlayerResolution)) {
		combine(mPlayerResolution, mPlayerClock);
		while(mPlayerClock.getNoOfCards() >= 7) { //Level up
			weissPlayer::levelUp();
		}
	}
	else {
		combine(mPlayerResolution, mPlayerWaitingRoom);
	}

}

void weissPlayer::levelUp(void) {

	mPlayerLevel.mContent.push_back(mPlayerClock.mContent.front()); //level first card of clock
	mPlayerClock.mContent.pop_front();
	int overFlow = mPlayerClock.getNoOfCards() - 6;

	for (int i = 0; i < overFlow; ++i) { //-6 since we leveled up one card, only 6 is supposed to go to waiting room
		mPlayerResolution.mContent.push_front(mPlayerClock.mContent.back());
		mPlayerClock.mContent.pop_back();
	}



	combine(mPlayerClock, mPlayerWaitingRoom);
	combine(mPlayerResolution, mPlayerClock);
}

weissDeck& weissPlayer::getClock(void) {
	return mPlayerClock;
}

weissDeck& weissPlayer::getLevel(void) {
	return mPlayerLevel;

}