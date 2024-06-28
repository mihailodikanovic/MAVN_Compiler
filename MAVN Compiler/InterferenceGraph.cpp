/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#include "InterferenceGraph.h"

using namespace std;


InterferenceGraph makeInterferenceGraph(Variables& varijable, Instructions& instrukcije) {

	Variables* vars = new Variables;

	for (auto it = varijable.begin(); it != varijable.end(); it++)  //prolazimo kroz sve varijable
	{
		if ((*it)->getType() == Variable::VariableType::REG_VAR)    //i trazimo samo registre
			vars->push_back((*it));    //punimo listu
	}

	InterferenceGraph ig(vars);

	ig.matrix = InterferenceMatrix(vars->size());

	for (int i = 0; i < vars->size(); i++) 
		ig.matrix[i].resize(vars->size());

	for (auto instIter = instrukcije.begin(); instIter != instrukcije.end(); instIter++) 
	{
		for (Variables::iterator defIt = ((*instIter)->getDef())->begin(); defIt != ((*instIter)->getDef())->end(); defIt++) 
		{
			for (Variables::iterator outIt = ((*instIter)->getOut())->begin(); outIt != ((*instIter)->getOut())->end(); outIt++) 
			{
				if (((*defIt)->getPosition() != (*outIt)->getPosition()) && (*defIt)->getPosition() != -1 && (*outIt)->getPosition() != -1) 
				{
					ig.matrix[(*defIt)->getPosition()][(*outIt)->getPosition()] = __INTERFERENCE__;
					ig.matrix[(*outIt)->getPosition()][(*defIt)->getPosition()] = __INTERFERENCE__;
				}
			}
		}
	}
	return ig;
}


void InterferenceGraph::printInterferenceMatrix() {
	if (matrix.size() == 0)
	{
		cout << "There is nothing to print!" << endl;
		return;
	}

	cout << "==========================================" << endl;
	cout << "Interference matrix:" << endl;
	cout << "==========================================" << endl;


	for (auto varIt = variables->begin(); varIt != variables->end(); varIt++)
	{
		cout << "\t" << (*varIt)->getName();
	}
	cout << endl;

	auto varIt = variables->begin();
	for (auto it1 = matrix.begin(); it1 != matrix.end(); it1++)
	{
		cout << (*varIt++)->getName();
		for (auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
		{
			cout << "\t" << *it2;
		}
		cout << endl;
	}
	cout << "==========================================" << endl;
}


InterferenceMatrix InterferenceGraph::getMatrix() 
{
	return matrix; 
}


Variables InterferenceGraph::getVars() 
{ 
	return *variables; 
}