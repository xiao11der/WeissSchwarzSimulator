#include <iostream>
#include <cstdio>
#include<string>
#include "cards.h"
#include "deck.h"
#include<chrono>
#include<vector>
#include<map>
#include "weissSimulation.h"
#include "weissPlayer.h"

int main(void) {

	//-------------------------USER INPUT-------------------------//
	// To be replaced by some form of GUI
	//Input Variable Declarations
	int noOfCardsInOppDeck;
	int noOfCxInOppDeck;
	int noOfCardsInOppWR;
	int noOfCxinOppWR;
	int tempAttackValue;
	std::vector<int> inputAttackSequence;
	
	std::cout << "Enter Number of Cards in Opponent Deck" << std::endl;
	std::cin >> noOfCardsInOppDeck;
	
	std::cout << "Enter Number of Cx in Opponent Deck" << std::endl;
	std::cin >> noOfCxInOppDeck;

	std::cout << "Enter Number of Cards in Opponent Waiting Room" << std::endl;
	std::cin >> noOfCardsInOppWR;

	std::cout << "Enter Number of Cx in Opponent Waiting Room" << std::endl;
	std::cin >> noOfCxinOppWR;

	std::cout << "Enter attack order, enter zero or non integer to stop" << std::endl;
	while ((std::cin >> tempAttackValue) && tempAttackValue != 0) {
		inputAttackSequence.push_back(tempAttackValue);
	}

	//-------------------------USER INPUT-------------------------//
	
	auto start = std::chrono::high_resolution_clock::now();//timing start

	const int MAX_ITER = 10000;

	
	//obtain game state

	
	std::map<int, int> damageChart; //Initialize damage chart for output
	std::vector<damageAction>damageSequence(inputAttackSequence.size(),damageAction(0));//


	weissSimulation weissTest; //Initialize Simulation
	weissPlayer selfInit(weissDeck(10, 3));//Initialize self deck state
	weissPlayer opponentInit(weissDeck(noOfCardsInOppDeck, noOfCxInOppDeck), weissDeck(noOfCardsInOppWR, noOfCxinOppWR)); //Initialize  opponent deck state and WR state


	int i = 0;
	for (auto dmg: inputAttackSequence) {
		damageSequence[i] = damageAction(dmg);
		++i;
	}

	 //
	

	weissTest.runWeissSimulation(MAX_ITER,damageChart,selfInit,opponentInit,damageSequence);



	//Report results
	for (auto i = damageChart.begin(); i != damageChart.end();++i) { //output damage results
		double damage = i->first; //map is inherently a pair, as such, use iter-> first and iter->second to get key and value
		double hitCount = i->second;

		double hitRate = 100 * hitCount / MAX_ITER;
		std::cout.precision(4);
		std::cout << "Damage: " << damage <<"  " << "Chance :" << hitRate << "%" << std::endl;//output damage distribution

	}

	



	auto stop = std::chrono::high_resolution_clock::now();//Stop timer

	std::cout.precision(4);
	std::cout << "Run Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() <<"ms"<< std::endl; //output run time
	

	
}