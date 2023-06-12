#include "effectAction.h"

burnX::burnX(int amount) : mAmount(amount) {

}


void burnX::performAction(weissPlayer& self, weissPlayer& opponent, std::deque<effectAction*>& onCancel) {

	if (!opponent.takeDamage(mAmount)) { // if opponent cancels, perform on cancel action, but the on cancel sequence does not get passed onto it
		std::deque<effectAction*> emptyCancel = std::deque<effectAction*>(); //I need an empty vector that signifies no action to be taken, there may be a better implementation
		while (onCancel.size() > 0) {
			onCancel.front()->performAction(self, opponent, emptyCancel);
			onCancel.pop_front();
		}
	};

}


pythonBurn::pythonBurn(std::string pyFile, deckReportIn reportInstructions, reportTgt tgt) : mPyfile(pyFile), mReportInstructions(reportInstructions), mTgt(tgt) {

}

void pythonBurn::performAction(weissPlayer& self, weissPlayer& opponent, std::deque<effectAction*>& onCancel) {

	// ------------------------------------------------------Scrap Paper Section [Psuedo Code]----------------------------------------------//
	// 
	// pythonBurn::performAction -- allow user to define their own python file to define action, implements composition based burns, where the damage taken is only based on composition of cards
	// TO-DO Need to think about if it can be generalized for more effects...e.g. hina, moca, zombieland finisher...etc
	// collect information on how the user wants to examine the deck using the createReportXCards function, e.g. 3 cards from top of self deck, can only do one deck
	// Run createReportXCards function, obtain results
	// Run python function with predefined name, running with the .raw field from the report as input
	// The python function should accept string as an input, parse string, and return damage instances
	// Return Option Ideas (pick one):
	// 1. The python function should return an array of number/boolean pairs,each representing an instance, the number is amount of damage,  boolean is whether or not that instance is cancellable
	// 2. The python function should return a string, with the damages and "cancellability" chained, and deliminated by some predefined deliminter
	// 3. The python will return a N*2 array of ints, where the first column of array represents damage, second one represents whether or not its cancellable (T=0, F=1)
	// 4. Return a N*2 int array, where the first column is a int that gives the action, where the second column is ONE parameter (e.g. x damage, or x amount to shuffle back)
	// 5. Return a N length array of strings, with commas seperating arguments, and the first word being the keyword to be interpreted
	// Loop through returned array, and execute burnX or topXToClock
	// https://docs.python.org/release/2.6.5/extending/embedding.html#pure-embedding
	// https://docs.python.org/3/c-api/intro.html
	// https://stackoverflow.com/questions/3286448/calling-a-python-method-from-c-c-and-extracting-its-return-value
	//
	// ------------------------------------------------------Scrap Paper Section----------------------------------------------//


	switch (mTgt) {
	case SELF:
		mReportResult = self.createReportXCards(mReportInstructions.x, mReportInstructions.peekSide, mReportInstructions.postReportAction);
		break;
	case OPPONENT:
		mReportResult = opponent.createReportXCards(mReportInstructions.x, mReportInstructions.peekSide, mReportInstructions.postReportAction);
		break;
	default:
		throw std::invalid_argument("Wrong target to generate deck report, must be SELF or OPPONENT");
	}


	PyObject *pFile, *pModule, * pFuncName, * pFunc;
	PyObject* pOutput, * pArgs;

	//Start Python Interpreter
	Py_Initialize();
	//These lines are needed such that the working directory (where the c++ code is) can be added, and PyImport can actually find the python file.
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
	PyRun_SimpleString("sys.path.append(os.getcwd() + './Effects')");

	pFile = PyUnicode_DecodeFSDefault((char*)mPyfile.c_str());

	if (pFile == NULL) {
		throw std::runtime_error("Error in python import, could not translate file name");
	}

	pModule = PyImport_Import(pFile);

	if (pModule == NULL) {
		throw std::runtime_error(std::format("Error in python import, could not load file, file name is : {}", mPyfile));
	}

	pFunc = PyObject_GetAttrString(pModule, (char*)"damageCalculation");

	if (!pFunc || !PyCallable_Check(pFunc)) {
		if (PyErr_Occurred()) {
			PyErr_Print();
		}
		throw std::runtime_error("Could not call function");

	}

	pArgs = PyTuple_New(1);
	PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(mReportResult.raw.c_str()));
	pOutput = PyObject_CallObject(pFunc, pArgs);


	auto outputSize = PyTuple_Size(pOutput);



	//parse arguments
	for (int i = 0; i < outputSize; ++i) {
		PyObject* currItem = PyTuple_GetItem(pOutput, i);
		PyObject* byteArr = PyUnicode_AsUTF8String(currItem);
		std::string pOutputString = std::string(PyBytes_AsString(byteArr));
		std::pair<std::string, std::string> action = parseParam(pOutputString, ',');

		if (!_stricmp(action.first.c_str(), "burn")) {
			if (!opponent.takeDamage(std::stoi(action.second))) { // if opponent cancels, perform on cancel action, but the on cancel sequence does not get passed onto it
				std::deque<effectAction*> emptyCancel = std::deque<effectAction*>(); //I need an empty vector that signifies no action to be taken, there may be a better implementation
				while (onCancel.size() > 0) {
					onCancel.front()->performAction(self, opponent, emptyCancel);
					onCancel.pop_front();
				}
			};
		}
		else if (!_stricmp(action.first.c_str(), "shuffle")) {
			opponent.shuffleBackNonCX(std::stoi(action.second));
		}
		else if (!_stricmp(action.first.c_str(), "uncancellable")) {
			opponent.takeUncanDamage(std::stoi(action.second));
		}
		else {
			throw std::invalid_argument(std::format("Unknown command from Python code:{}", action.first));
		}

	}



	Py_XDECREF(pFile);
	Py_XDECREF(pFunc);
	Py_XDECREF(pModule);
	if (Py_FinalizeEx() < 0) {
		throw std::runtime_error("Python failed to exit");
	}
}