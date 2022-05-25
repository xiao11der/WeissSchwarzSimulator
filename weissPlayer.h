#pragma once
#include ".\deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>

class weissPlayer {

public:
	
	weissPlayer(weissDeck deckState); //Minimal Ctor

	weissPlayer(weissDeck deckState,
		weissDeck waitingRoomState, 
		weissDeck clockState, 
		weissDeck levelState, 
		weissDeck stockState,
		weissDeck memoryState); //Ctor with full specificity

	weissPlayer(weissPlayer& p1); //Copy ctor
	void combine(weissDeck& deckSource, weissDeck& deckDest);
	
	weissDeck& getLevel(void);
	weissDeck& getClock(void);

	
	void takeDamage(int amount);
	void shuffleDeck(void);
	void levelUp(void);

private:
	weissDeck mPlayerDeck;
	weissDeck mPlayerStock;
	weissDeck mPlayerClock;
	weissDeck mPlayerWaitingRoom;
	weissDeck mPlayerMemory;
	weissDeck mPlayerLevel;
	weissDeck mPlayerResolution;

};