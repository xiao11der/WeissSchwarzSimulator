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


pythonBurn::pythonBurn(std::string pyFile, deckReportIn reportInstructions) : mPyfile(pyFile), mReportInstructions(reportInstructions) {

}

void pythonBurn::performAction(weissPlayer& self, weissPlayer& opponent, std::deque<effectAction*>& onCancel) {

	// ------------------------------------------------------Scrap Paper Section [Psuedo Code]----------------------------------------------//
	// 
	// pythonBurn::performAction -- allow user to define their own python file to define action, implements composition based burns, where the damage taken is only based on composition of cards
	// TO-DO Need to think about if it can be generalized for more effects...e.g. hina, moca, zombieland finisher...etc
	// collect information on how the user wants to examine the deck using the createReportXCards function, e.g. 3 cards from top of self deck, can only do one deck
	// Run createReportXCards function, obtain results
	// Run python function with predefined name, running with the .raw field from the report as input
	// The python function should accept string as an input, parse string, and return damage instances
	// Return Option Ideas (pick one):
	// 1. The python function should return an array of number/boolean pairs,each representing an instance, the number is amount of damage,  boolean is whether or not that instance is cancellable
	// 2. The python function should return a string, with the damages and "cancellability" chained, and deliminated by some predefined deliminter
	// 3. The python will return a N*2 array of ints, where the first column of array represents damage, second one represents whether or not its cancellable (T=0, F=1)
	// Loop through returned array, and execute burnX or topXToClock
	// https://docs.python.org/release/2.6.5/extending/embedding.html#pure-embedding
	// https://docs.python.org/3/c-api/intro.html
	// https://stackoverflow.com/questions/3286448/calling-a-python-method-from-c-c-and-extracting-its-return-value
	//
	// ------------------------------------------------------Scrap Paper Section----------------------------------------------//






}