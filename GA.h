#ifndef GA_H
#define GA_H

#include <vector>

using namespace std;

vector<int> generateRandomPopulation(int individuals, int numVariables);

void GA_MAXSAT(vector< vector<int>> clauses, int individuals, string selectionMethod, string crossOverMethod, double pC, double pM, int generations, int numVariables);

#endif
