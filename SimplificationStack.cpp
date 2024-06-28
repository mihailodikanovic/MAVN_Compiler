/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#include "SimplificationStack.h"

using namespace std;

SimplificationStack* doSimplification(InterferenceGraph ig, int degree) 
{
	InterferenceMatrix matrix = ig.getMatrix();
	Variables vars = ig.getVars();

	SimplificationStack* stack = new SimplificationStack;

	int position = -1;
	int maxInterference;

	while (!vars.empty()) 
	{
		bool found = false;

		maxInterference = -1;
		for (int i = 0; i < matrix.size(); i++) 
		{
			int interference = 0;
			for (int j = 0; j < matrix.size(); j++) 
			{
				if (matrix[i][j] == __INTERFERENCE__) interference++;
			}
			if (interference > maxInterference && interference < degree) 
			{
				position = i;
				found = true;
			}

		}
		if (found) 
		{
			matrix.erase(matrix.begin() + position);
			
			for (unsigned i = 0; i < matrix.size(); ++i)
			{
				matrix[i].erase(matrix[i].begin() + position);
			}
			
			for (auto it = vars.begin(); it != vars.end(); it++) 
			{
				if ((*it)->getPosition() == position) 
				{
					stack->push(*it);
					vars.erase(it);
					break;
				}
			}
		}

		if (!found) 
			throw NotEnughRegisters();

	}
	
	return stack;

}