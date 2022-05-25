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

	auto start = std::chrono::high_resolution_clock::now();//timing start

	const int MAX_ITER = 10000;

	
	//obtain game state

	
	std::map<int, int> damageChart; //Initialize damage chart for output
	
	weissSimulation weissTest;
	weissPlayer selfInit(weissDeck(10, 3));
	weissPlayer opponentInit(weissDeck(37, 7));
	std::vector<damageAction>damageSequence(4,damageAction(2));
	

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