#pragma once
#include <vector>
#include "cards.h"
#include "deck.h"
#include <string>
#include<stdexcept>
#include<algorithm>
#include <map>
#include "weissPlayer.h"
#include "Python.h"
#include<iostream>

class effectAction {

public:

	virtual void performAction(weissPlayer& self, weissPlayer& opponent, std::deque<effectAction*>& onCancel) = 0; //Abstract class, all effect actions should be derived classes, and effect action can only be passed by pointer, not value


};

class burnX : public effectAction { //single instance of burn X

public:
	burnX(int amount);

	void performAction(weissPlayer& self, weissPlayer& opponent, std::deque<effectAction*>& onCancel);


private:
	int mAmount;

};



class pythonBurn : public effectAction {

public:

	pythonBurn(std::string pyFile, deckReportIn reportInstructions);

	void performAction(weissPlayer& self, weissPlayer& opponent, std::deque<effectAction*>& onCancel);

private:
	std::string mPyfile;
	deckReportIn mReportInstructions;
	deckReport mReportResult;
};
