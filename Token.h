#pragma once

#include "Constants.h"
#include "Types.h"

#include <vector>
#include <string>

using namespace std;

class Token
{
private:
	
	TokenType tokenType;
	
	
	string value;
	
	
	string tokenTypeToString(TokenType t);

public:

	Token(void);

	~Token(void);
	
	
	TokenType getType();

	
	void setType(TokenType t);
	
	
	string getValue();
	
	
	void setValue(string s);


	void makeToken(int begin, int end, std::vector<char>& program, int lastFiniteState);
	
	
	void makeErrorToken(int pos, std::vector<char>& program);
	
	
	void makeEofToken();
	
	
	void printTokenInfo();

	
	void printTokenValue();
};

