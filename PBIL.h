#ifndef PBIL_H
#define PBIL_H

#include <vector>

using namespace std;

void printSolution(int* solution, int stringSize);

void printPopulation(int** population, int size, int stringSize);

void printPV(vector<double> PV);

void mutatePV(vector<double> &PV, double pM, double mutAmnt);

int* evalFitness(int** population, int size, int stringSize);

int findMaxFitness(int* fitnessList, int size);

int findMinFitness(int* fitnessList, int size);

void PBIL_MAXSAT(int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations, int stringSize);


#endif
