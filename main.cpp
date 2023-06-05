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
#include<stdexcept>
#include"parser.h"

int main(void) {


	
	
	
	
	//-------------------------USER INPUT-------------------------//
	// To be replaced by some form of GUI
	// GUI NOTES:
	// Pass to main functions via a string, it should have the following information: a. config (iterations and such), b. game state, c. attack sequence and their effects
	//Input Variable Declarations
	int noOfCardsInOppDeck;
	int noOfCxInOppDeck;
	int noOfCardsInOppWR;
	int noOfCxInOppWR;
	int tempAttackValue;
	int MAX_ITER;
	std::vector<int> inputAttackSequence;
	std::vector<damageAction> damageSequence;


	//---------------------INPUT FROM FILE-----------------------//
	std::string inputFile = "TestInputFile.txt";
	try {
		gameStruct currGame = parseFile(inputFile);
		std::cout << "Reading from File: " << inputFile << std::endl;
		noOfCardsInOppDeck = currGame.noOfCardsInOppDeck;
		noOfCxInOppDeck = currGame.noOfCxInOppDeck;
		noOfCardsInOppWR = currGame.noOfCardsInOppWR;
		noOfCxInOppWR = currGame.noOfCxInOppWR;
		damageSequence = currGame.damageSequence;
		MAX_ITER = currGame.MAX_ITER;
		std::cout << "Read Complete" << std::endl;

	}
	catch(std::exception& err) {
		std::cout << "Standard Exception During Input Parse: " << err.what() << std::endl;
		return -1;
	}
	//---------------------INPUT FROM FILE-----------------------//

	
	//-----------------CMD-------------------------------------------//

	//std::cout << "Enter Number of Cards in Opponent Deck" << std::endl;
	//std::cin >> noOfCardsInOppDeck;

	//std::cout << "Enter Number of Cx in Opponent Deck" << std::endl;
	//std::cin >> noOfCxInOppDeck;

	//std::cout << "Enter Number of Cards in Opponent Waiting Room" << std::endl;
	//std::cin >> noOfCardsInOppWR;

	//std::cout << "Enter Number of Cx in Opponent Waiting Room" << std::endl;
	//std::cin >> noOfCxInOppWR;

	//-----------------CMD-------------------------------------------//






	//Input Error Handling
	try {
		if (noOfCardsInOppDeck <0 || noOfCxInOppDeck < 0 || noOfCardsInOppWR<0 || noOfCxInOppWR<0) {
			throw std::invalid_argument(std::string("Invalid Deck - Card numbers cannot be negative! \n") +
				std::string("Card in Opp Deck : ") + std::to_string(noOfCardsInOppDeck) + std::string("\n") +
				std::string("CX in Opp Deck : ") + std::to_string(noOfCxInOppDeck) + std::string("\n") +
				std::string("Card in Opp WR : ") + std::to_string(noOfCardsInOppWR) + std::string("\n") +
				std::string("CX in Opp WR : ") + std::to_string(noOfCxInOppWR) + std::string("\n")
			);
		}


		if (noOfCxInOppWR + noOfCxInOppDeck > 8) {
			throw std::invalid_argument(std::string("Invalid Deck, too many Cxs, there should be less than 8 Cxs in the deck, the total Cx count appears to be ") + std::to_string(noOfCxInOppWR + noOfCxInOppDeck));
		}

		if (noOfCardsInOppDeck + noOfCardsInOppWR != 50) {
			throw std::invalid_argument(std::string("Invalid Deck, card count should be exactly 50, card count appears to be ") + std::to_string(noOfCardsInOppDeck + noOfCardsInOppWR));
		}


	}
	catch (std::exception & err) {
		std::cout << "Standard Exception: " << err.what() << std::endl;
		return 1;
	}

	//-----------------CMD-------------------------------------------//
	//std::cout << "Enter attack order, enter zero or non integer to stop" << std::endl;
	//while ((std::cin >> tempAttackValue) && tempAttackValue != 0) {
	//	inputAttackSequence.push_back(tempAttackValue);
	//}
	//-----------------CMD-------------------------------------------//

	//-------------------------USER INPUT-------------------------//
	
	auto start = std::chrono::high_resolution_clock::now();//timing start

	

	
	//obtain game state

	try {

		std::cout << "Setting up Weiss Game.." << std::endl;

		std::map<int, int> damageChart; //Initialize damage chart for output
		weissSimulation weissTest; //Initialize Simulation
		weissPlayer selfInit(weissDeck(10, 3));//Initialize self deck state
		weissPlayer opponentInit(weissDeck(noOfCardsInOppDeck, noOfCxInOppDeck), weissDeck(noOfCardsInOppWR, noOfCxInOppWR)); //Initialize  opponent deck state and WR state

		//--------------------------------CMD POST PROCESSING---------------------------------------//
		
		//std::vector<damageAction>damageSequence(inputAttackSequence.size(), damageAction(0, false));//

		////TO-DO: Add config options class to simulation with options to run arbitrary amount of iterations, and to either run with old or new refresh rules, modify tests accordingly
		//


		////Initialize variables for effects
		//
		//std::deque<effectAction*> onAttack = std::deque<effectAction*>();
		//
		//std::deque<effectAction*> onCancel = std::deque<effectAction*>();
		//
		//std::deque<effectAction*> postAttack = std::deque<effectAction*>();

		//int i = 0;
		//
		////--------TEST CODE FOR ON ATTACK BURN 1 AND/OR ON CANCEL BURN 1---------//
		//burnX* burn1 = new burnX(1);
		//onCancel.push_back(burn1);
		////--------TEST CODE FOR ON ATTACK BURN 1 AND/OR ON CANCEL BURN 1---------//


		//

		//for (auto dmg : inputAttackSequence) {
		//	damageSequence[i] = damageAction(dmg, onAttack, onCancel, postAttack, true);
		//	++i;
		//}

		//--------------------------------CMD POST PROCESSING---------------------------------------//

		//Run simulation
		std::cout << "Running Weiss Simulation for: " << MAX_ITER << " iterations" << std::endl;
		weissTest.runWeissSimulation(MAX_ITER, damageChart, selfInit, opponentInit, damageSequence);




		//Report results
		for (auto i = damageChart.begin(); i != damageChart.end(); ++i) { //output damage results
			double damage = i->first; //map is inherently a pair, as such, use iter-> first and iter->second to get key and value
			double hitCount = i->second;

			double hitRate = 100 * hitCount / MAX_ITER;
			std::cout.precision(4);
			std::cout << "Damage: " << damage << "  " << "Chance :" << hitRate << "%" << std::endl;//output damage distribution

		}
	}
	catch (std::exception& e) {//Use standard exceptions to report errors
		std::cout << e.what() << std::endl;
		return 1;
	}

	



	auto stop = std::chrono::high_resolution_clock::now();//Stop timer

	std::cout.precision(4);
	std::cout << "Run Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() <<"ms"<< std::endl; //output run time
	
	return 0;
	
}