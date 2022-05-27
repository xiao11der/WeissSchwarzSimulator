#include "damageAction.h"

damageAction::damageAction(int amount, bool performTrigger, std::vector<std::shared_ptr<effectAction>> preAttackActionSequence, std::vector<std::shared_ptr<effectAction>> onCancelActionSequence, std::vector<std::shared_ptr<effectAction>> postAttackActionSequence)
	:
	mAmount(amount),
	mTrigger(performTrigger),
	mPreAttackAction(preAttackActionSequence),
	mOnCancelAction(onCancelActionSequence),
	mPostAttackAction(postAttackActionSequence)
{
}

damageAction::damageAction(int amount, bool performTrigger)
	:
	mAmount(amount),
	mTrigger(performTrigger)
{
}


int damageAction::performDamageAction(weissPlayer& self, weissPlayer& opponent) {

	//perform pre attack effects
	//perform trigger, calculate attack damage
	//perform attack on opponent deck
	//perform on cancel effects
	//perform post attack effects

	int initialDamage = opponent.getLevel().getNoOfCards()*7+opponent.getClock().getNoOfCards(); //Initial opponent damage

	opponent.takeDamage(mAmount);

	int finalDamage= opponent.getLevel().getNoOfCards() * 7 + opponent.getClock().getNoOfCards(); //final opponent damage

	int totalDamage = finalDamage - initialDamage;

	return totalDamage;



}

