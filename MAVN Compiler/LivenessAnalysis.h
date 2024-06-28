/* Autor: Mihailo Dikanovic Datum: 05.06.2023. */

#pragma once

#include "Types.h"
#include "Constants.h"
#include "IR.h"
#include <list>
#include <iterator>

void fillSuccPred(Instructions& instrukcije);

void fillDefUse(Instructions& instrukcije);

void livenessAnalysis(Instructions& instrukcije);