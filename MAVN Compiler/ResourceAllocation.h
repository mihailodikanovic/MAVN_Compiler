/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#pragma once

#include "Constants.h"
#include "InterferenceGraph.h"
#include "SimplificationStack.h"

#include<map>
#include<list>


bool doResourceAllocation(SimplificationStack ss, InterferenceGraph ig);


bool checkResourceAllocation(InterferenceGraph ig);
