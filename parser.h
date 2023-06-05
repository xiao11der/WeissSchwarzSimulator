#pragma once
#include<fstream>
#include<string>
#include<iostream>
#include<stdexcept>
#include<utility>
#include<format>
#include<vector>
#include"damageAction.h"


#define SIMPLE 0

struct gameStruct {
	int MAX_ITER = 0;
	int check_triggers = false;


	int selfGameState = -1;
	int noOfCardsInDeck = 0;
	int noOfTriggersInDeck = 0;
	int noOfCardsInWR = 0;

	int oppGameState = -1;
	int noOfCardsInOppDeck=0;
	int noOfCxInOppDeck=0;
	int noOfCardsInOppWR=0;
	int noOfCxInOppWR =0;

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
std::pair<std::string, std::string> parseParam(std::string line);
void trim(std::string& line);