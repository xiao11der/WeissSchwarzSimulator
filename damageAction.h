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


class damageAction {

public:
	
	damageAction(int amount, bool performTrigger,
		std::vector<std::shared_ptr<effectAction>> preAttackActionSequence,
		std::vector<std::shared_ptr<effectAction>> onCancelActionSequence,
		std::vector<std::shared_ptr<effectAction>> postAttackActionSequence
	);

	damageAction(int amount, bool performTrigger = false
	);
	
	int performDamageAction(weissPlayer& self, weissPlayer& opponent);


private:
	int mAmount;
	bool mTrigger;
	std::vector<std::shared_ptr<effectAction>> mPreAttackAction;
	std::vector<std::shared_ptr<effectAction>> mPostAttackAction;
	std::vector<std::shared_ptr<effectAction>> mOnCancelAction;


};