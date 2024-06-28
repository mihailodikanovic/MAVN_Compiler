#pragma once

#include <map>
#include <string>

#include "Constants.h"
#include "Types.h"

typedef std::map<int, std::map<char,int>> STATE_MACHINE;

class FiniteStateMachine
{
private:
	
	STATE_MACHINE stateMachine;

	
	static const TokenType stateToTokenTable[NUM_STATES];

	
	static const char supportedCharacters[NUM_OF_CHARACTERS];


	static const int stateMatrix[NUM_STATES][NUM_OF_CHARACTERS];

public:
	
	int getNextState(int currentState, char transitionLetter);

	
	void initStateMachine();

	
	static TokenType getTokenType(int stateNumber);
};



