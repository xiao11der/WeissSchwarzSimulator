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
	
	/*damageAction(int amount, bool performTrigger = false,
		std::vector<std::unique_ptr<effectAction>> preAttackActionSequence= std::vector<std::unique_ptr<effectAction>>(1,std::make_unique<effectAction>("none")),
		std::vector<std::unique_ptr<effectAction>> onCancelActionSequence = std::vector<std::unique_ptr<effectAction>>(1, std::make_unique<effectAction>("none")),
		std::vector<std::unique_ptr<effectAction>> postAttackActionSequence = std::vector<std::unique_ptr<effectAction>>(1, std::make_unique<effectAction>("none"))
	);*/

	damageAction(int amount, bool performTrigger = false
	);
	
	int performDamageAction(weissPlayer& self, weissPlayer& opponent);


private:
	int mAmount;
	bool mTrigger;
	//std::vector<std::unique_ptr<effectAction>> mPreAttackAction;
	//std::vector<std::unique_ptr<effectAction>> mPostAttackAction;
	//std::vector<std::unique_ptr<effectAction>> mOnCancelAction;


};