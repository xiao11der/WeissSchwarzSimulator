#pragma once
#include<fstream>
#include<string>
#include<iostream>
#include<stdexcept>
#include<utility>
#include<format>
#include<vector>
#include"damageAction.h"
#include"effectAction.h"
#include "common.h"

#define SIMPLE 0

struct gameStruct {
	int MAX_ITER = 0;
	int check_triggers = false;


	int selfGameState = -1;
	int noOfCardsInDeck = -1;
	int noOfTriggersInDeck = -1;
	int noOfCardsInWR = -1;
	int noOfCXInDeck = -1;
	int noOfCXInWR = -1;

	int oppGameState = -1;
	int noOfCardsInOppDeck=-1;
	int noOfCxInOppDeck=-1;
	int noOfCardsInOppWR=-1;
	int noOfCxInOppWR =-1;

	std::vector<damageAction> damageSequence;
	
};

struct attackStruct {

	int soul = -999;
	int startingLine = -1;

	std::string step = "NONE";
	std::string effect = "NONE";
	std::deque<effectAction*>* currArrayPointer = nullptr;

	std::deque<effectAction*> onAttack = std::deque<effectAction*>();

	std::deque<effectAction*> onCancel = std::deque<effectAction*>();

	std::deque<effectAction*> postAttack = std::deque<effectAction*>();
};


gameStruct parseFile(std::string inputFile);

void swapEffectState(std::string state_name, attackStruct& currAttack);
