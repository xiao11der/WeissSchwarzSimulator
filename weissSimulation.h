#pragma once
#include <vector>
#include "cards.h"
#include "deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>
#include <map>
#include "weissPlayer.h"
#include "damageAction.h"
#include "effectAction.h"

class weissSimulation {

public:
	weissSimulation(){};
	bool runWeissSimulation(int iterationCount, std::map<int, int>& damageChart, weissPlayer selfInit, weissPlayer opponentInit, std::vector<damageAction> damageSequence);

private:

};