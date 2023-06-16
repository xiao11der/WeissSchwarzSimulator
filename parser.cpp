#include "parser.h"

gameStruct parseFile(std::string inputFile) {

	std::ifstream inFile(inputFile);
	std::string line;
	std::string section = "NONE";
	gameStruct game;
	attackStruct currAttack;
	std::pair<std::string, std::string> currentParam;
	int lineCount = 0;


	//Flags for multi-variable parts like python
	// 
	//Python import
	bool fPython = false;
	bool fileDone = false, amountDone = false, playerDone = false, peekSideDone = false, postActionDone = false;
	deckReportIn dummy_struct;
	std::string file = "";
	reportTgt tgt;
	postReport postReportAction;
	peekPos pos;
	int amount;

	//TO-DO: Change game state initialization to flag-based checks too


	while (std::getline(inFile, line)) {

		lineCount++;

		if (line.rfind("//", 0) == 0 || line == "") {
			continue;
		}

		trim(line);
	

		//Look for end of section first
		if (line.rfind("##", 0) == 0) {
			if (section != "NONE") {

				if (section == "ATTACK") {
					game.damageSequence.push_back(damageAction(currAttack.soul, currAttack.onAttack, currAttack.onCancel, currAttack.postAttack, game.check_triggers));
				}

				section = "NONE";


			}
			else {
				throw std::runtime_error(std::format("Line {}: Syntax error in input file, section end (##) encountered without proper header. Line Content: {}", lineCount, line));
			}
		}

		//Look for section headers
		
		if (line.rfind("#CONFIG", 0) == 0) {
			if (section == "NONE") {
				section = "CONFIG";
			}
			else {
				throw std::runtime_error(std::format("Line {}: Syntax error in input file, section end (##) not found for Section {}.", lineCount, section));
			}
			continue; //Skip the current line
		}
		else if (line.rfind("#SELF", 0) == 0) {
			if (section == "NONE") {
				section = "SELF";
			}
			else {
				throw std::runtime_error(std::format("Line {}: Syntax error in input file, section end (##) not found for Section {}.", lineCount, section));
			}
			continue; //Skip the current line
		}
		else if (line.rfind("#OPPONENT", 0) == 0) {
			if (section == "NONE") {
				section = "OPPONENT";
			}
			else {
				throw std::runtime_error(std::format("Line {}: Syntax error in input file, section end (##) not found for Section {}.", lineCount, section));
			}
			continue; //Skip the current line
		}
		else if (line.rfind("#ATTACK", 0) == 0) {
			if (section == "NONE") {
				section = "ATTACK";
				currAttack = {};
				currAttack.soul = 0;
				currAttack.startingLine = lineCount;
			}
			else {
				throw std::runtime_error(std::format("Line {}: Syntax error in input file, section end (##) not found for Section {}.", lineCount, section));
			}
			continue; //Skip the current line
		}
		else if (line.rfind("#", 0) == 0 && line.rfind("##", 0) != 0) { //If a single # is detected, but not #, and if it doesnt match one of the valid sections above
			throw std::invalid_argument(std::format("Line {}: Invalid Section. Line Content: {}", lineCount, line));
		}
		

		if (section == "CONFIG") {
			//std::cout << "Parsing Config" << std::endl;
			currentParam = parseParam(line, '=');

			if (!_stricmp(currentParam.first.c_str(), "max_iter")) { //strcmp returns 0 for equal strings
					game.MAX_ITER = std::stoi(currentParam.second);
			}
			else if (!_stricmp(currentParam.first.c_str(), "trigger")) {
				if (!_stricmp(currentParam.second.c_str(), "true")) {
					game.check_triggers = true;
				}
				else if (!_stricmp(currentParam.second.c_str(), "false")) {
					game.check_triggers = false;
					
				}
				else {
					throw std::invalid_argument("The arugment for trigger check must be true or false, what you entered is " + currentParam.second);
				}

			}
			else {
				throw std::invalid_argument("Undefined argument in #CONFIG, what you entered is " + currentParam.first);

			}

		}

		if (section == "SELF") {
			//std::cout << "Parsing self state" << std::endl;
			currentParam = parseParam(line, '=');
			if (!_stricmp(currentParam.first.c_str(), "game_state_type")) {
				if (!_stricmp(currentParam.second.c_str(), "simple")) {
					game.selfGameState = SIMPLE;
				}
				else {
					throw std::invalid_argument("The arugment for game state must be one out of the following: 1. simple, what you entered is " + currentParam.second);
				}
			}
			else if (!_stricmp(currentParam.first.c_str(), "no_of_cards_in_deck")) {
				game.noOfCardsInDeck = std::stoi(currentParam.second);
			}
			else if (!_stricmp(currentParam.first.c_str(), "no_of_trigger_in_deck")) {
				game.noOfTriggersInDeck = std::stoi(currentParam.second);
			}
			else if (!_stricmp(currentParam.first.c_str(), "no_of_cards_in_wr")) {
				game.noOfCardsInWR = std::stoi(currentParam.second);
			}
			else if (!_stricmp(currentParam.first.c_str(), "no_of_cx_in_deck")) {
				game.noOfCXInDeck = std::stoi(currentParam.second);
			}
			else if (!_stricmp(currentParam.first.c_str(), "no_of_cx_in_wr")) {
				game.noOfCXInWR = std::stoi(currentParam.second);
			}
			else {
				throw std::invalid_argument("Undefined argument in #SELF, what you entered is " + currentParam.first);
			}
		}

		if (section == "OPPONENT") {
			//std::cout << "Parsing opponent state" << std::endl;
			currentParam = parseParam(line, '=');
			if (!_stricmp(currentParam.first.c_str(), "game_state_type")) {
				if (!_stricmp(currentParam.second.c_str(), "simple")) {
					game.selfGameState = SIMPLE;
				}
				else {
					throw std::invalid_argument("The arugment for game state must be one out of the following: 1. simple, what you entered is " + currentParam.second);
				}
			}
			else if (!_stricmp(currentParam.first.c_str(), "no_of_cards_in_deck")) {
				game.noOfCardsInOppDeck = std::stoi(currentParam.second);
			}
			else if (!_stricmp(currentParam.first.c_str(), "no_of_cx_in_deck")) {
				game.noOfCxInOppDeck = std::stoi(currentParam.second);
			}
			else if (!_stricmp(currentParam.first.c_str(), "no_of_cards_in_wr")) {
				game.noOfCardsInOppWR = std::stoi(currentParam.second);
			}
			else if (!_stricmp(currentParam.first.c_str(), "no_of_cx_in_wr")) {
				game.noOfCxInOppWR = std::stoi(currentParam.second);
			}
			else {
				throw std::invalid_argument("Undefined argument in #OPPONENT, what you entered is " + currentParam.first);
			}
		}

		if (section == "ATTACK") {
			//std::cout << "Parsing attack" << std::endl;
			if (currAttack.soul < -100) {
				throw std::runtime_error(std::format("Line: {}: Attack parsing is incorrect, attack structure not correctly resetted for the section, soul is too low [Unitialized soul is -999]",lineCount));
			}


			if (line.rfind("%%", 0) == 0) {
				if (currAttack.step != "NONE") {
					currAttack.step = "NONE";
					continue;
				}
				else {
					throw std::runtime_error(std::format("Line {}: Syntax error in input file, attack section end (%%) encountered without proper header for attack that starts on line {}. Line Content: {}", lineCount, currAttack.startingLine, line));
				}
			}



			if (line.rfind("%OnAttack", 0) == 0) {
				if (currAttack.step == "NONE") {
					currAttack.step = "OnAttack";
					currAttack.currArrayPointer = &(currAttack.onAttack);
					continue;
				}
				else {
					throw std::runtime_error(std::format("Line {}: Syntax error in input file, attack section end not found (%%) for step {} in  attack that starts on line {}. Line Content: {}", lineCount, currAttack.step, currAttack.startingLine, line));
				}
			}
			else if (line.rfind("%OnCancel", 0) == 0) {
				if (currAttack.step == "NONE") {
					currAttack.step = "OnCancel";
					currAttack.currArrayPointer = &(currAttack.onCancel);
					continue;
				}
				else {
					throw std::runtime_error(std::format("Line {}: Syntax error in input file, attack section end not found (%%) for step {} in  attack that starts on line {}. Line Content: {}", lineCount, currAttack.step, currAttack.startingLine, line));
				}
				
			}
			else if (line.rfind("%PostAttack", 0) == 0) {
				if (currAttack.step == "NONE") {
					currAttack.step = "PostAttack";
					currAttack.currArrayPointer = &(currAttack.postAttack);
					continue;
				}
				else {
					throw std::runtime_error(std::format("Line {}: Syntax error in input file, attack section end not found (%%) for step {} in  attack that starts on line {}. Line Content: {}", lineCount, currAttack.step, currAttack.startingLine, line));
				}
			}
			else if (line.rfind("%", 0) == 0 && line.rfind("%%", 0) != 0) {
				throw std::invalid_argument(std::format("Line {}: Invalid attack step keyword. Line Content: {}", lineCount, line));
			}


			if (line.rfind("**", 0) == 0) {
				if (currAttack.effect != "NONE") {
					currAttack.effect = "NONE";
					continue;
				}
				else {
					throw std::runtime_error(std::format("Line {}: Syntax error in input file, effect section end (**) encountered without proper header for attack that starts on line {}. Line Content: {}", lineCount, currAttack.startingLine, line));
				}

			}

			if (line.rfind("*BURNX", 0) == 0) {
				swapEffectState("BurnX", currAttack);
				continue;
			}
			else if (line.rfind("*PYTHON", 0) == 0) {
				swapEffectState("Python", currAttack);
				if (fPython) {
					throw std::runtime_error(std::format("Line{}: Python initialized but never finished, make sure all the parameters are set", lineCount));
				}
				//reset all flags
				fPython = true;
				fileDone = false;
				amountDone = false;
				playerDone = false;
				peekSideDone = false;
				postActionDone = false;
				continue;
			}
			else if (line.rfind("*AVATAR", 0) == 0) {
				swapEffectState("Avatar", currAttack);
				continue;
			}
			else if (line.rfind("*SILICA", 0) == 0) {
				swapEffectState("Silica", currAttack);
				currAttack.currArrayPointer->push_front(new silicaBurn());
				continue;
			}
			else if (line.rfind("*", 0) == 0 && line.rfind("**", 0) != 0) {
				throw std::invalid_argument(std::format("Line {}: Invalid effect keyword. Line Content: {}", lineCount, line));
			}

			if (currAttack.effect == "BurnX") {
				int amount = -999;

				currentParam = parseParam(line, '=');
				if (!_stricmp(currentParam.first.c_str(), "amount")) {
					amount = std::stoi(currentParam.second);
				}

				if (amount != -999) {
					currAttack.currArrayPointer->push_front(new burnX(amount));

				}
			}

			if (currAttack.effect == "Avatar") {
				bool supp = false;
				currentParam = parseParam(line, '=');
				if (!_stricmp(currentParam.first.c_str(), "support")) {
					if (!_stricmp(currentParam.second.c_str(), "true")) {
						supp = true;
					}
					else if (!_stricmp(currentParam.second.c_str(), "false")) {
						supp = false;
					}
					else {
						throw std::invalid_argument("Wrong avatar support selection, must be TRUE or FALSE");
					}

					currAttack.currArrayPointer->push_front(new avatarBurn(supp));
					
				}

			}

			if (currAttack.effect == "Silica") {
				throw std::runtime_error("This should not have been reached");
			}

			if (currAttack.effect == "Python") {

				currentParam = parseParam(line, '=');
				
				if (!_stricmp(currentParam.first.c_str(), "file")) {
					file = currentParam.second;
					fileDone = true;
				}
				else if (!_stricmp(currentParam.first.c_str(), "amount")) {
					amount = std::stoi(currentParam.second);
					amountDone = true;
				}
				else if (!_stricmp(currentParam.first.c_str(), "player")) {
					std::string peekReportTgt;
					peekReportTgt = currentParam.second;
					if (!_stricmp(peekReportTgt.c_str(), "self")) {
						tgt = SELF;
					}
					else if (!_stricmp(peekReportTgt.c_str(), "opponent")) {
						tgt = OPPONENT;
					}
					else {
						throw std::invalid_argument(std::format("Wrong player for python read in, must be self or opponent, the parsed input:{}", peekReportTgt));
					}
					playerDone = true;
				}
				else if (!_stricmp(currentParam.first.c_str(), "post_check")) {
					std::string postReportString;
					postReportString = currentParam.second;
					if (!_stricmp(postReportString.c_str(), "to_wr")) {
						postReportAction = WR;
					}
					else if (!_stricmp(postReportString.c_str(), "to_top")) {
						postReportAction = TOPDECK;
					}
					else if (!_stricmp(postReportString.c_str(), "to_bottom")) {
						postReportAction = BOTDECK;
					}
					else if (!_stricmp(postReportString.c_str(), "return_and_shuffle")) {
						postReportAction = DECKSHUFFLE;
					}
					else {
						throw std::invalid_argument(std::format("Wrong post check action, must be to_wr, to_top, to_bottom, or return_and_shuffle, the parsed input:{}", postReportString));
					}
					peekSideDone = true;
				}
				else if (!_stricmp(currentParam.first.c_str(), "position")) {
					std::string peekPosString;
					peekPosString = currentParam.second;
					if (!_stricmp(peekPosString.c_str(), "top")) {
						pos = TOP;
					}
					else if (!_stricmp(peekPosString.c_str(), "bottom")) {
						pos = BOTTOM;
					}
					else {
						throw std::invalid_argument(std::format("Wrong peek position for python read in, must be top or bottom, the parsed input:{}", peekPosString));
					}
					postActionDone = true;
				}
				else {
					throw std::invalid_argument(std::format("Line{}: Wrong argument for python.", lineCount));
				}

				if (fileDone && amountDone && playerDone && peekSideDone && postActionDone) {
					dummy_struct.peekSide = pos;
					dummy_struct.postReportAction = postReportAction;
					dummy_struct.x = amount;
					currAttack.currArrayPointer->push_front(new pythonBurn(file, dummy_struct, tgt));
					fPython = false;
				}


			}

			//Base attack parameters not including effects
			if (currAttack.effect == "NONE" && currAttack.step == "NONE") { //If its not currently parsing an attack step or an effect
				currentParam = parseParam(line, '=');
				if (!_stricmp(currentParam.first.c_str(), "soul")) { //strcmp returns 0 for equal strings
					currAttack.soul = std::stoi(currentParam.second);
				}
			}



		}


		//std::cout << line << std::endl;


	}

	return game;

}

void swapEffectState(std::string state_name, attackStruct& currAttack) {
	if (currAttack.effect == "NONE") {
		currAttack.effect = state_name;
	}
	else {
		throw std::runtime_error(std::format("Effect state swap failed for {} for attack step {} of attack starting on line {}, was the previous state properly terminated with **?",state_name,currAttack.step,currAttack.startingLine));
	}
}