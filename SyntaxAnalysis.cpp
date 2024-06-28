/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#include "SyntaxAnalysis.h"
#include "IR.h"
#include <iostream>

using namespace std;


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex, Variables& vars, Labels& labs, Instructions& instrs): lexicalAnalysis(lex), errorFound(false),
	varijable(vars), labele(labs), instrukcije(instrs)
{
	tokenIterator = lexicalAnalysis.getTokenList().begin();
	varPozicija = 0;
}


bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();
	Q();

	if (--tokenIterator == lexicalAnalysis.getTokenList().begin())
		return false;
	else
		return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token& token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (currentToken.getType() == t)
	{
		currentToken.printTokenValue();
		if (t != T_END_OF_FILE)
			currentToken = getNextToken();
	}
	else
	{
		printSyntaxError(currentToken);
		errorFound = true;
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}


Variable* SyntaxAnalysis::getVariable(string name)
{
	for (auto it = varijable.begin(); it != varijable.end(); it++)
	{
		if ((*it)->getName() == name)
		{
			return *it;
		}
	}
}


void SyntaxAnalysis::Q()
{
	if (errorFound) 
		return;
	
	S();
	if (currentToken.getType() == T_SEMI_COL) 
	{
		eat(T_SEMI_COL);
		L();
	}
	else 
	{
		errorFound = true;
		printSyntaxError(currentToken);
	}
}


void SyntaxAnalysis::S()
{	

	if (errorFound) 
		return;
	
	if (currentToken.getType() == T_MEM) 
	{
		eat(T_MEM);
		if (currentToken.getType() == T_M_ID) 
		{
			Variable* var = new Variable(currentToken.getValue(), -1,  Variable::MEM_VAR);
			eat(T_M_ID);
			if (currentToken.getType() == T_NUM) 
			{
				var->setValue(currentToken.getValue());
				varijable.push_back(var);
				eat(T_NUM);
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_REG) 
	{
		eat(T_REG);
		if (currentToken.getType() == T_R_ID) 
		{
			Variable* var = new Variable(currentToken.getValue(), varPozicija++, Variable::REG_VAR);
			varijable.push_back(var);
			eat(T_R_ID);
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_FUNC) 
	{
		eat(T_FUNC);
		if (currentToken.getType() == T_ID) 
		{
			Label* lab = new Label(currentToken.getValue());
			labele.push_back(lab);
			eat(T_ID);
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_ID) 
	{
		Label* lab = new Label(currentToken.getValue());
		labele.push_back(lab);
		eat(T_ID);
		if (currentToken.getType() == T_COL) 
		{
			eat(T_COL);
			E();
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else 
	{
		E();
	}
}


void SyntaxAnalysis::L()
{
	if (errorFound) 
		return;
	
	if (currentToken.getType() == T_END_OF_FILE) 
	{
		eat(T_END_OF_FILE);
	}
	else
	{
		Q();
	}
}


void SyntaxAnalysis::E()
{
	if (errorFound) 
		return;

	Variables dsts, srcs;
	
	//1 ADD
	if (currentToken.getType() == T_ADD) 
	{
		eat(T_ADD);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_R_ID) 
				{
					srcs.push_back(getVariable(currentToken.getValue()));
					eat(T_R_ID);
					if (currentToken.getType() == T_COMMA) 
					{
						eat(T_COMMA);
						if (currentToken.getType() == T_R_ID) 
						{
							srcs.push_back(getVariable(currentToken.getValue()));
							eat(T_R_ID);
							
							instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, I_ADD, dsts, srcs));
							dsts.clear();
							srcs.clear();
						}
						else 
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else 
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//2 ADDI
	else if (currentToken.getType() == T_ADDI) {
		eat(T_ADDI);
		if (currentToken.getType() == T_R_ID) {
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_R_ID) 
				{
					srcs.push_back(getVariable(currentToken.getValue()));
					eat(T_R_ID);
					if (currentToken.getType() == T_COMMA) 
					{
						eat(T_COMMA);
						if (currentToken.getType() == T_NUM) 
						{
							int num = stoi(currentToken.getValue());
							eat(T_NUM);
							instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, num, I_ADDI, dsts, srcs));
							dsts.clear();
							srcs.clear();
						}
						else 
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else 
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//3 SUB
	else if (currentToken.getType() == T_SUB) 
	{
		eat(T_SUB);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_R_ID) 
				{
					srcs.push_back(getVariable(currentToken.getValue()));
					eat(T_R_ID);
					if (currentToken.getType() == T_COMMA) 
					{
						eat(T_COMMA);
						if (currentToken.getType() == T_R_ID) 
						{
							srcs.push_back(getVariable(currentToken.getValue()));
							eat(T_R_ID);
							instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, I_SUB, dsts, srcs));
							dsts.clear();
							srcs.clear();
						}
						else
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else 
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//4 LA
	else if (currentToken.getType() == T_LA) 
	{
		eat(T_LA);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) {
				eat(T_COMMA);
				if (currentToken.getType() == T_M_ID) 
				{
					srcs.push_back(getVariable(currentToken.getValue()));
					eat(T_M_ID);
					instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, I_LA, dsts, srcs));
					dsts.clear();
					srcs.clear();
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//5 LW
	else if (currentToken.getType() == T_LW) {
		eat(T_LW);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_NUM) 
				{
					int num = stoi(currentToken.getValue());
					eat(T_NUM);
					if (currentToken.getType() == T_L_PARENT) 
					{
						eat(T_L_PARENT);
						if (currentToken.getType() == T_R_ID) 
						{
							srcs.push_back(getVariable(currentToken.getValue()));
							eat(T_R_ID);
							if (currentToken.getType() == T_R_PARENT) 
							{
								eat(T_R_PARENT);
								instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, num, I_LW, dsts, srcs));
								dsts.clear();
								srcs.clear();
							}
							else 
							{
								errorFound = true;
								printSyntaxError(currentToken);
							}
						}
						else 
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else 
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//6 LI
	else if (currentToken.getType() == T_LI) 
	{
		eat(T_LI);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_NUM) 
				{
					int num = stoi(currentToken.getValue());
					eat(T_NUM);
					instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, num, I_LI, dsts, srcs));
					dsts.clear();
					srcs.clear();
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//7 SW
	else if (currentToken.getType() == T_SW) 
	{
		eat(T_SW);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_NUM) 
				{
					int num = stoi(currentToken.getValue());
					eat(T_NUM);
					if (currentToken.getType() == T_L_PARENT) 
					{
						eat(T_L_PARENT);
						if (currentToken.getType() == T_R_ID) 
						{
							srcs.push_back(getVariable(currentToken.getValue()));
							eat(T_R_ID);
							if (currentToken.getType() == T_R_PARENT) 
							{
								eat(T_R_PARENT);
								instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, num, I_SW, dsts, srcs));
								dsts.clear();
								srcs.clear();
							}
							else 
							{
								errorFound = true;
								printSyntaxError(currentToken);
							}
						}
						else 
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else 
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	 
	//8 B
	else if (currentToken.getType() == T_B) 
	{
		eat(T_B);
		if (currentToken.getType() == T_ID) 
		{
			string s = currentToken.getValue();
			eat(T_ID);
			instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, s, I_B, dsts, srcs));
			dsts.clear();
			srcs.clear();
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//9 BLTZ
	else if (currentToken.getType() == T_BLTZ) 
	{
		eat(T_BLTZ);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_ID) 
				{
					string s = currentToken.getValue();
					eat(T_ID);
					instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, s, I_BLTZ, dsts, srcs));
					dsts.clear();
					srcs.clear();
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//10 NOP
	else if (currentToken.getType() == T_NOP) {
		eat(T_NOP);
		instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, I_NOP, dsts, srcs));
	}
	
	//11 AND
	else if (currentToken.getType() == T_AND) {
		eat(T_AND);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_R_ID) 
				{
					srcs.push_back(getVariable(currentToken.getValue()));
					eat(T_R_ID);
					if (currentToken.getType() == T_COMMA) 
					{
						eat(T_COMMA);
						if (currentToken.getType() == T_R_ID) 
						{
							srcs.push_back(getVariable(currentToken.getValue()));
							eat(T_R_ID);
							instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, I_AND, dsts, srcs));
							dsts.clear();
							srcs.clear();
						}
						else 
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else 
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//12 OR
	else if (currentToken.getType() == T_OR) 
	{
		eat(T_OR);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_R_ID) 
				{
					srcs.push_back(getVariable(currentToken.getValue()));
					eat(T_R_ID);
					if (currentToken.getType() == T_COMMA) 
					{
						eat(T_COMMA);
						if (currentToken.getType() == T_R_ID) 
						{
							srcs.push_back(getVariable(currentToken.getValue()));
							eat(T_R_ID);
							instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, I_OR, dsts, srcs));
							dsts.clear();
							srcs.clear();
						}
						else 
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else 
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	
	//13 SLT
	else if (currentToken.getType() == T_SLT) 
	{
		eat(T_SLT);
		if (currentToken.getType() == T_R_ID) 
		{
			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA) 
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_R_ID) 
				{
					srcs.push_back(getVariable(currentToken.getValue()));
					eat(T_R_ID);
					if (currentToken.getType() == T_COMMA) 
					{
						eat(T_COMMA);
						if (currentToken.getType() == T_R_ID)
						{
							srcs.push_back(getVariable(currentToken.getValue()));
							eat(T_R_ID);
							instrukcije.push_back(new Instruction((*labele.rbegin())->getLabelName(), instrukcije.size() + 1, I_SLT, dsts, srcs));
							dsts.clear();
							srcs.clear();
						}
						else 
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else 
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else 
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else 
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else 
	{
		errorFound = true;
		printSyntaxError(currentToken);
	}
	
}


void SyntaxAnalysis::printVariables(Variables& varijable) 
{
	cout << endl;
	cout <<"==========================================" << endl;
	cout << "Variables after resource allocation:" << endl;
	cout << "==========================================" << endl;

	for (auto it = varijable.begin(); it != varijable.end(); it++)
	{
		cout << "[NAME] " << (*it)->getName() << " [TYPE] " << (*it)->typeToS((*it)->getType()) << " [ASSIGNMENT] " << regsToS((*it)->getAssignment()) << endl;
	}

	cout << "==========================================" << endl;
	cout << endl;
}