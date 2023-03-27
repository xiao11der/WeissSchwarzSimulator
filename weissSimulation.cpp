#include "weissSimulation.h"

bool weissSimulation::runWeissSimulation(int iterationCount, std::map<int, int>& damageChart, weissPlayer selfInit, weissPlayer opponentInit, std::vector<damageAction> damageSequence) {
	//Run simulation for fixed iteration count
	//TO-DO: Add convergence based metric?
	for (int i = 0; i < iterationCount; ++i) {
		//Initialize Player objects, shuffle decks, and init output
		weissPlayer self(selfInit);
		weissPlayer opponent(opponentInit);

		self.shuffleDeck();
		opponent.shuffleDeck();

		int dmg = 0;
		//Perform damage using game rules
		for (damageAction damageInstance : damageSequence) {
			dmg+=damageInstance.performDamageAction(self, opponent);
		}
		//Record damage
		damageChart[dmg]++;

	}
	//TO-DO: Think about and add error conditions for "false"
	return true;

}

