/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#include "Types.h"
#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "LivenessAnalysis.h"
#include "InterferenceGraph.h"
#include "SimplificationStack.h"
#include "ResourceAllocation.h"

#include <iostream>
#include <exception>
#include <string>


using namespace std;


void Analysis(string& in, Variables& v, Labels& l , Instructions& i)
{	

	string fileName = in;
	
	bool retVal = false;




	// LexicalAnalysis //

	LexicalAnalysis lex;

	if (!lex.readInputFile(fileName))
		throw runtime_error("\nException! Failed to open input file!\n");

	lex.initialize();

	retVal = lex.Do();

	if (retVal)
	{
		cout << "Lexical analysis finished successfully!" << endl;
		lex.printTokens();
	}
	else
	{
		lex.printLexError();
		throw runtime_error("\nException! Lexical analysis failed!\n");
	}




	// SyntaxAnalysis //

	SyntaxAnalysis syntax(lex, v, l, i);

	retVal = syntax.Do();

	if (retVal)
	{
		cout << endl << endl << "Syntax analysis finished successfully!" << endl << endl << endl;
	}
	else
	{
		cout << "Syntax analysis failed!" << endl;
	}




	// ControlGraph //

	fillDefUse(i);
	
	fillSuccPred(i);




	// LivenessAnalysis //
	
	livenessAnalysis(i);




	// InterferenceGraph //

	InterferenceGraph ig = makeInterferenceGraph(v, i);
	ig.printInterferenceMatrix();




	// SimplificationStack //

	SimplificationStack* stack = doSimplification(ig, __REG_NUMBER__);




	// ResourceAllocation //

	doResourceAllocation(*stack, ig);
	if (checkResourceAllocation(ig)) 
	{
		cout << "\n\nResource allocation is successfull!\n" << endl;
		syntax.printVariables(v);
	}
	else 
		throw runtime_error("\n\nException! Resource allocation failed!\n");
}




void Ispis(string& out, Variables& v, Labels& l, Instructions& i)
{

	//Pravimo output fajl
	string putanja = ".\\..\\examples\\";
	string outputName = putanja + out;
	ofstream outFile(outputName);
	
	if (!outFile.is_open())
	{
		cout << "Input or output file \"" << outputName << "\" could not be opened." << endl;
	}
	

	outFile << ".globl main" << endl << endl;
	
	//Ispisuju se memorijske promenljive
	outFile << ".data" << endl;
	for (auto m = v.begin(); m != v.end(); m++)
	{
		if ((*m)->getType() == Variable::VariableType::MEM_VAR)
			outFile << (*m)->getName() << ":  .word  " << (*m)->getValue() << endl;
	}
	

	//Ispisuju se instrukcije ispod odredjene labele
	outFile << endl << ".text" << endl;

	auto lit = l.begin();
	
	for (auto it = i.begin(); it != i.end(); it++)
	{
		if ((*it)->getLabela() != (*lit)->getLabelName())   // ako se polje labela unutar instrukcije poklapa sa nazivom labele - ispisujemo instrukciju 
		{                                                   // ako se ne poklapaju trazimo novu labelu
			lit++;
			outFile << (*lit)->getLabelName() << ":" << endl;
		}


		outFile << "	" << (*it)->printInstruction();     //naziv instrukcije


		//instrukcije tipa  naziv rid, rid, rid
		if ((*it)->getType() == I_ADD || (*it)->getType() == I_SUB || (*it)->getType() == I_AND ||  
			(*it)->getType() == I_OR || (*it)->getType() == I_SLT)
		{
			auto dsts = (*it)->getDst()->begin();
			auto srcs = (*it)->getSrc()->begin();

			outFile << " " << regsToS((*dsts)->getAssignment()) << ", ";
			outFile << regsToS((*srcs)->getAssignment()) << ", ";
			srcs++;
			outFile << regsToS((*srcs)->getAssignment()) << endl;
	
		}

		//instrukcija tipa  naziv rid, rid, num
		else if ((*it)->getType() == I_ADDI)
		{
			auto dsts = (*it)->getDst()->begin();
			auto srcs = (*it)->getSrc()->begin();

			outFile << " " << regsToS((*dsts)->getAssignment()) << ", ";
			outFile << regsToS((*srcs)->getAssignment()) << ", ";
			outFile << (*it)->returnKonstanta() << endl;
		}

		//instrukcije tipa  naziv rid, num(rid)
		else if ((*it)->getType() == I_LW || (*it)->getType() == I_SW)
		{
			auto dsts = (*it)->getDst()->begin();
			auto srcs = (*it)->getSrc()->begin();

			outFile << " " << regsToS((*dsts)->getAssignment()) << ", ";
			outFile << (*it)->returnKonstanta() << "(" << regsToS((*srcs)->getAssignment()) << ")" << endl;
		}

		//instrukcija tipa  naziv rid, mid
		else if ((*it)->getType() == I_LA)
		{
			auto dsts = (*it)->getDst()->begin();
			auto srcs = (*it)->getSrc()->begin();

			outFile << " " << regsToS((*dsts)->getAssignment()) << ", ";
			outFile << (*srcs)->getName() << endl;
			
		}

		//instrukcija tipa  naziv rid, num
		else if ((*it)->getType() == I_LI)
		{
			auto dsts = (*it)->getDst()->begin();

			outFile << " " << regsToS((*dsts)->getAssignment()) << ", ";
			outFile << (*it)->returnKonstanta() << endl;
		}

		//instrukcija tipa  naziv rid, id
		else if ((*it)->getType() == I_BLTZ)
		{
			auto dsts = (*it)->getDst()->begin();

			outFile << " " << regsToS((*dsts)->getAssignment()) << ", ";
			outFile << (*it)->returnJump() << endl;

		}

		//instrukcija tipa  naziv id
		else if ((*it)->getType() == I_B)
		{
			outFile << (*it)->returnJump() << endl;
		}
		else
		{
			outFile << endl;
		}
	}

	outFile.close();
}
	



int main()
{
	string putanja = ".\\..\\examples\\";
	string fajl;
	

	//interakcija sa konzolom da ne bismo morali svaki put da menjamo naziv fajla u kodu
	cout << "Unesite naziv fajla -----> "; 
	cin >> fajl;
		

	string in = putanja + fajl;   //ime ulaznog fajla (naziv.mavn)
	string out = fajl.substr(0, fajl.size() - 4) + "s";  //ime izlaznog fajla  (naziv.s)


	Variables vars;
	Labels labs;
	Instructions instrs;


	try
	{
		Analysis(in, vars, labs, instrs);
		Ispis(out, vars, labs, instrs);
		
		return 0;
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}
}