#include "damageAction.h"

damageAction::damageAction(int amount, std::deque<effectAction*> preAttackActionSequence, std::deque<effectAction*> onCancelActionSequence, std::deque<effectAction*> postAttackActionSequence, bool performTrigger)
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

	while (mPreAttackAction.size() > 0) { //resolve all preattack queue
		mPreAttackAction.front()->performAction(self, opponent, mOnCancelAction);
		mPreAttackAction.pop_front();
	}

	if (!opponent.takeDamage(mAmount)) { //resolve main damage, if cancel occurs, resolve oncancel sequence
		while (mOnCancelAction.size() > 0) {
			std::deque<effectAction*> emptyCancel = std::deque<effectAction*>(); //I should find a more elegant way to do this
			mOnCancelAction.front()->performAction(self, opponent, emptyCancel);
			mOnCancelAction.pop_front();
		}

	};

	int finalDamage= opponent.getLevel().getNoOfCards() * 7 + opponent.getClock().getNoOfCards(); //final opponent damage

	int totalDamage = finalDamage - initialDamage;

	return totalDamage;



}

