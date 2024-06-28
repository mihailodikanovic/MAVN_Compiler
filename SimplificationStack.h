/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#pragma once

#include "IR.h"
#include "InterferenceGraph.h"

#include <stack>


typedef std::stack<Variable*> SimplificationStack;


SimplificationStack* doSimplification(InterferenceGraph ig, int degree);


struct NotEnughRegisters : std::runtime_error 
{
	NotEnughRegisters() : runtime_error("\n\nThere is not enough registers to complete the program!\n\n") {}
};