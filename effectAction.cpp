#include "effectAction.h"

burnX::burnX(int amount) : mAmount(amount) {

}

void burnX::performAction(weissPlayer& self, weissPlayer& opponent, std::deque<effectAction*>& onCancel) {

	if (!opponent.takeDamage(mAmount)) { // if opponent cancels, perform on cancel action, but the on cancel sequence does not get passed onto it
		std::deque<effectAction*> emptyCancel = std::deque<effectAction*>(); //I need an empty vector that signifies no action to be taken, there may be a better implementation
		while (onCancel.size() > 0) {
			onCancel.front()->performAction(self, opponent, emptyCancel);
			onCancel.pop_front();
		}
	};

}