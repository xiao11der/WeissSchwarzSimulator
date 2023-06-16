#pragma once
#include ".\deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>
#include<map>
#include<format>

struct deckReport {
	int noOfCx = 0;
	std::map<int, int> levelDist;
	std::map<int, int> triggerDist;
	std::map<cardColor, int> colorDist;
	std::string raw = "";
};
enum peekPos { TOP, BOTTOM };
enum postReport { WR, DECKSHUFFLE, TOPDECK, BOTDECK};
enum stockWashMode {TOWR, TODECK};

struct deckReportIn {
	int x;
	peekPos peekSide;
	postReport postReportAction;
};


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

	weissPlayer(const weissPlayer& p1); //Copy ctor
	void combine(weissDeck& deckSource, weissDeck& deckDest);


	//Getters
	weissDeck& getLevel(void);
	weissDeck& getClock(void);
	int getCurrentSoul(void);

	//Player actions that directly correspond to game actions
	bool takeDamage(int amount);
	void takeUncanDamage(int amount);
	void shuffleBackNonCX(int amount); //shuffle up to X non-climax cards back into the deck
	void stockWash(stockWashMode mode);
	bool burnDeck(int damage, bool canBeCanceled);
	void blindStock(int amount);
	void takeRefreshDamage(void);
	void shuffleDeck(void);
	void levelUp(void);
	void triggerCheck(void);
	
	//Helper Functions
	deckReport createReportXCards(int x, peekPos peekSide, postReport postReportAction);


	

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