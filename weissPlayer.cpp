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

weissPlayer::weissPlayer(weissPlayer& p1)

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
			if (topCard.getType() == "climax") {
				return false; //damage canacled
			}
		}
	}
	return true;
}

void weissPlayer::levelUp(void) {

	for (auto i = mPlayerClock.mContent.begin(); i <= mPlayerClock.mContent.begin()+6; ++i) { //Iterate the first 7 cards of the clock, find card to level up, level up the first non-cx
		card topClock = mPlayerClock.mContent.front();
		if (topClock.getType() != "climax") {
			mPlayerLevel.mContent.push_back(topClock);
			mPlayerClock.mContent.pop_front();
			//std::cout << "found non-cx" << std::endl;
			break;
		}

		if (i == mPlayerClock.mContent.begin() + 5) { //If the entire clock is cx, just level a cx
			mPlayerLevel.mContent.push_back(topClock);
			mPlayerClock.mContent.pop_front();
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

