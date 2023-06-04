#pragma once
#include <vector>
#include "cards.h"
#include "deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>
#include <map>
#include "effectAction.h"
#include"weissPlayer.h"
#include<memory>
#include<deque>


class damageAction {

public:
	
	damageAction(int amount, //Ctor with all effects
		std::deque<effectAction*> preAttackActionSequence = std::deque<effectAction*>(),
		std::deque<effectAction*> onCancelActionSequence = std::deque<effectAction*>(),
		std::deque<effectAction*> postAttackActionSequence = std::deque<effectAction*>(),
		bool performTrigger = false
	);

	damageAction(int amount, bool performTrigger = false //Ctor with just amount and trigger
	);
	
	int performDamageAction(weissPlayer& self, weissPlayer& opponent);


private:
	int mAmount;
	bool mTrigger;
	std::deque<effectAction*> mPreAttackAction;
	std::deque<effectAction*> mPostAttackAction;
	std::deque<effectAction*> mOnCancelAction;


};