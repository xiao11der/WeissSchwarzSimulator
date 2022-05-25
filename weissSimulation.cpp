#include "weissSimulation.h"

bool weissSimulation::runWeissSimulation(int iterationCount, std::map<int, int>& damageChart, weissPlayer selfInit, weissPlayer opponentInit, std::vector<damageAction> damageSequence) {

	for (int i = 0; i < iterationCount; ++i) {

		weissPlayer self(selfInit);
		weissPlayer opponent(opponentInit);

		self.shuffleDeck();
		opponent.shuffleDeck();

		int dmg = 0;

		for (damageAction damageInstance : damageSequence) {
			dmg+=damageInstance.performDamageAction(self, opponent);
		}

		damageChart[dmg]++;

	}

	return true;

}

