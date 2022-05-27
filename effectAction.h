#pragma once
#include <vector>
#include "cards.h"
#include "deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>
#include <map>
#include "weissPlayer.h"

class effectAction {

public:
	effectAction(std::string name): mName(name){};

	virtual void performAction(weissPlayer& self, weissPlayer& opponent) {
	};


private:
	std::string mName;
	

};
