#include "weissSimulation.h"

void weissSimulation::runWeissSimulation(int iterationCount, std::map<int, int>& damageChart, weissPlayer selfInit, weissPlayer opponentInit, std::vector<damageAction> damageSequence) {
	//Run simulation for fixed iteration count
	//TO-DO: Add convergence based metric?
	for (int i = 0; i < iterationCount; ++i) {
		//Initialize Player objects, shuffle decks, and init output
		weissPlayer self(selfInit);
		weissPlayer opponent(opponentInit);

		//std::vector<weissPlayer> opponentDebugSnapshot;
		//std::vector<weissPlayer> selfDebugSnapshot;
		//opponentDebugSnapshot.clear();

		self.shuffleDeck();
		opponent.shuffleDeck();

		//opponentDebugSnapshot.push_back(opponent);
		//selfDebugSnapshot.push_back(self);

		int dmg = 0;
		//Perform damage using game rules
		for (damageAction damageInstance : damageSequence) {
			dmg+=damageInstance.performDamageAction(self, opponent);

			//opponentDebugSnapshot.push_back(opponent);
			//selfDebugSnapshot.push_back(self);

		}
		//Record damage
		damageChart[dmg]++;

	}

}

