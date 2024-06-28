/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#pragma once
#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"
#include <list>

class SyntaxAnalysis
{
private:

	int varPozicija;  //pozicija varijable


	LexicalAnalysis& lexicalAnalysis;
	
	
	TokenList::iterator tokenIterator;


	Token currentToken;

	
	void Q();

	
	void S();

	
	void L();

	
	void E();


	bool errorFound;

public:
	
	Variables& varijable;
	Instructions& instrukcije;
	Labels& labele;


	SyntaxAnalysis(LexicalAnalysis& lex, Variables& vars, Labels& labs, Instructions& instrs);

	
	bool Do();

	
	void printSyntaxError(Token& token);

	
	void eat(TokenType t);

	
	Token getNextToken();


	Variable* getVariable(string name);


	void printVariables(Variables& varijable);
};

