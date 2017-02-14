#ifndef PBIL_H
#define PBIL_H

#include <vector>

using namespace std;

void printSolution(int* solution, int numVariables);

void printPopulation(int** population, int size, int numVariables);

void printPV(vector<double> PV);

void mutatePV(vector<double> &PV, double pM, double mutAmnt);

int countSatClauses(vector< vector<int> > clauses, int* solution);

int* evalFitness(vector< vector<int> > clauses, int** population, int size, int numVariables);

int findMaxFitness(int* fitnessList, int size);

int findMinFitness(int* fitnessList, int size);

void PBIL_MAXSAT(vector< vector<int> > clauses, int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations, int numVariables);

#endif
