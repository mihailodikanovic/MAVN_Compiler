/* Autor: Mihailo Dikanovic Datum: 07.06.2023. */

#pragma once

#include "IR.h"


typedef std::vector<std::vector<int>> InterferenceMatrix;


class InterferenceGraph 
{
public:
	Variables* variables;
	InterferenceMatrix matrix;	

	InterferenceGraph() {}

	InterferenceGraph(Variables* v) : variables(v) {}

	void setMatrix(InterferenceMatrix* mat) { matrix = *mat; }

	void printInterferenceMatrix();

	InterferenceMatrix getMatrix();

	Variables getVars();

};

InterferenceGraph makeInterferenceGraph(Variables& varijable, Instructions& instrukcije);