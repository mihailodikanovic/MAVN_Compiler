/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#include "ResourceAllocation.h"

using namespace std;

Variables izbacene;
list<Regs> slobodni_registri;

bool doResourceAllocation(SimplificationStack ss, InterferenceGraph ig) 
{

	// napraviti registre, proci kroz stek i napraviti nekoliko promena
	while (!ss.empty())
	{	
		slobodni_registri.push_back(t3);
		slobodni_registri.push_back(t2);
		slobodni_registri.push_back(t1);
		slobodni_registri.push_back(t0);
		
		Variable* v = ss.top();  //uzimamo promenljivu sa vrha steka
		ss.pop(); 

		if (!izbacene.empty()) 
		{
			for (Variables::iterator iter = izbacene.begin(); iter != izbacene.end(); iter++)  //prolazimo kroz izbacene varijable
			{
				if (ig.matrix[v->getPosition()][(*iter)->getPosition()] == __INTERFERENCE__) //ako su u smetnji
				{
					slobodni_registri.remove((*iter)->getAssignment()); //izbacujemo registar iz slobodnih registara
				}
			}
		}
		izbacene.push_back(v);  //izbacujemo proverenu promenljivu
		
		if (slobodni_registri.empty()) 
			return false;  //ako nema vise slobodnih registara prekidamo izvrsavanje
		else 
		{
			Regs reg = slobodni_registri.back();  //uzimamo slobodan registar iz liste
			slobodni_registri.clear();  //brisemo taj registar iz liste
			v->setAssignment(reg);  //dodeljujemo ga promenljivoj
		}

	} //vrsimo petlju dok god ima promenljivih na steku
	
	izbacene.clear();
	return true; 
}

bool checkResourceAllocation(InterferenceGraph ig) 
{
	Variable var1, var2;

	for (int i = 0; i < ig.matrix.size(); i++) 
	{
		for (int j = 0; j < ig.matrix.size(); j++)
			
			if (ig.matrix[i][j] == __INTERFERENCE__)   //ako su u smetnji
			{

				for (auto it = ig.variables->begin(); it != ig.variables->end(); it++) 
				{
					if ((*it)->getPosition() == i) //varijabla iz i-te vrste
					{ 
						var1 = *(*it); 
					}

					if ((*it)->getPosition() == j) //i varijabla iz j-te kolone
					{ 
						var2 = *(*it); 
					}
				}

				if (var1.getAssignment() == var2.getAssignment())
					return false;  //ne smeju imati isti resurs
			}
	}
	return true;

}