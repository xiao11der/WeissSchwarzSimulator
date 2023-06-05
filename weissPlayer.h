#pragma once
#include ".\deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>

class weissPlayer {

public:

	weissPlayer(weissDeck deckState); //Minimal Ctor
	weissPlayer(weissDeck deckState, weissDeck waitingRoomState); //Minimal Ctor with waiting room

	weissPlayer(weissDeck deckState,
		weissDeck waitingRoomState,
		weissDeck clockState,
		weissDeck levelState,
		weissDeck stockState,
		weissDeck memoryState); //Ctor with full specificity

	weissPlayer(weissPlayer& p1); //Copy ctor
	void combine(weissDeck& deckSource, weissDeck& deckDest);


	//Getters
	weissDeck& getLevel(void);
	weissDeck& getClock(void);
	int getCurrentSoul(void);

	//Player actions that directly correspond to game actions
	bool takeDamage(int amount);
	void takeUncanDamage(int amount);
	bool burnDeck(int damage, bool canBeCanceled);
	void takeRefreshDamage(void);
	void shuffleDeck(void);
	void levelUp(void);
	void triggerCheck(void);
	

	//System functions needed to run simulator that occurs intrinsically in game
	void endOfAttack(void);


private:
	//Zones
	weissDeck mPlayerDeck;
	weissDeck mPlayerStock;
	weissDeck mPlayerClock;
	weissDeck mPlayerWaitingRoom;
	weissDeck mPlayerMemory;
	weissDeck mPlayerLevel;
	weissDeck mPlayerResolution;

	//Refresh flag
	bool mQueueRefreshDamage=false;

	//Additional soul counter
	int mAddSoul;

};