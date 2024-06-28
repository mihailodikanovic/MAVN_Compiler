/* Autor: Mihailo Dikanovic Datum: 05.06.2023. */

#include "LivenessAnalysis.h"

using namespace std;

void fillSuccPred(Instructions& instrukcije)
{
	auto it = instrukcije.begin();
	Instruction* instr = new Instruction();


	for (; it != instrukcije.end(); it++)
	{
		if (it != instrukcije.begin()) 
		{
			(*it)->setPred(*prev(it));
		}


		//bezuslovni skok
		if ((*it)->getType() == I_B)
		{
			for (auto label = instrukcije.begin(); label != instrukcije.end(); label++) 
			{
				if ((*label)->getLabela() == (*it)->getLabela() && (*label)->getType() == I_NO_TYPE)
				{
					(*it)->setSucc(*next(label));
					(*next(label))->setPred(*it);
				}
			}
		}
		

		//uslovni skok
		else if ((*it)->getType() == I_BLTZ)
		{
			for (auto label = instrukcije.begin(); label != instrukcije.end(); label++) 
			{
				if ((*label)->getType() == I_NO_TYPE && (*it)->getLabela() == (*label)->getLabela()) 
				{
					(*it)->setSucc(*next(label));
					(*next(label))->setPred(*it);
				}
			}

			if (next(it) != instrukcije.end())
				(*it)->setSucc(*next(it));

		}
		

		else
		{
			auto nextt = next(it, 1);
			
			if (next(it) != instrukcije.end())
				(*it)->setSucc(*next(it));
			
		}
	}
}




void fillDefUse(Instructions& instrukcije)
{
	for (auto it = instrukcije.begin(); it != instrukcije.end(); it++) 
	{
		Variables* dstVars = (*it)->getDst();
		Variables* useVars = (*it)->getSrc();

		for (auto var = dstVars->begin(); var != dstVars->end(); var++) 
		{
			(*it)->setDef(*var);
		}
		
		(*it)->getDef()->unique();

		for (auto var = useVars->begin(); var != useVars->end(); var++) 
		{
			(*it)->setUse(*var);
		}
		
		(*it)->getUse()->unique();

	}
}




bool isEqual(Variables first, Variables second) {
	bool ret = true;

	if (first.size() != second.size())
		return false;

	for (auto it = first.begin(), it2 = second.begin(); it != first.end(), it2 != second.end(); it++, it2++) 
	{
		if ((*it)->getName() != (*it2)->getName()) 
			ret = false;
	}
	
	return ret;
}




void livenessAnalysis(Instructions& instrukcije)
{
	Variables* in = new Variables;
	Variables* out = new Variables;
	
	Variables inP;
	Variables outP;


	Variables::iterator itV;
	Variables::iterator itV2;
	
	
	Instructions::iterator it2;
	

	do 
	{
		for (auto it = instrukcije.rbegin(); it != instrukcije.rend(); ++it) 
		{
			in = (*it)->getIn();
			out = (*it)->getOut();

			inP.clear();
			for (auto o = in->begin(); o != in->end(); o++)
				inP.push_back(*o);

			outP.clear();
			for (auto l = out->begin(); l != out->end(); l++) 
				outP.push_back(*l);

			it2 = (*it)->getSucc()->begin();
			
			for (; it2 != (*it)->getSucc()->end(); it2++)
			{
				for (itV = (*it2)->getIn()->begin(); itV != (*it2)->getIn()->end(); itV++)
				{
					out->push_back(*itV);
				}
			}
			
			out->sort();
			out->unique();
			
			for (itV = (*it)->getUse()->begin(); itV != (*it)->getUse()->end(); itV++)
			{
				in->push_back(*itV); 
			}
			
			bool minus;

			for (itV = out->begin(); itV != out->end(); itV++)
			{
				minus = true;
				
				for (itV2 = (*it)->getDef()->begin(); itV2 != (*it)->getDef()->end(); itV2++) 
				{
					if (*itV == *itV2)  
					{
						minus = false;
						break;
					}
				}
				
				if (minus) 
					in->push_back(*itV);
			}

			in->sort();
			in->unique();
		
		}

	} while (!isEqual(inP, *in) || !isEqual(outP, *out));

}