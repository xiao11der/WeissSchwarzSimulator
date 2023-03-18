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
	//TO-DO: Add ctor with config, player and damage sequence fields 
	//Add new public shell function, move runWeissSimulation to private
public:
	weissSimulation(){};
	bool runWeissSimulation(int iterationCount, std::map<int, int>& damageChart, weissPlayer selfInit, weissPlayer opponentInit, std::vector<damageAction> damageSequence);

private:

};