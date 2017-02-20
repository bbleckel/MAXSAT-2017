#ifndef MAXSAT_H
#define MAXSAT_H

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <ctime>
#include <climits>
#include <cmath>
#include <fstream>


using namespace std;

class MaxSat {

public:
    // constructor/destructor
    MaxSat(string fileName, int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations);
	MaxSat(string fileName, int individuals, string selection, string crossover, double pC, double pM, int generations);
    ~MaxSat();
    
    void readFile(string fileName);

    
    void initPV();
	void initPopulation();

    // fitness
    int countSatClauses(int* solution);
    void evalFitness();
    int findMaxFitness();
    int findMinFitness();

	// selection
	void selectRanking();
	void selectTournament();
	void arrayCopy(int* arr1, int* arr2, int size);
	void selectBoltzman();

	// crossover
	void onePCross();
	void uniformCross();

    // mutation
    void mutatePV();
	void mutateOffspring();

    // printing
    void printSolution(int* solution);
    void printPopulation();
    void printPV();
    void printClauses();

    void solvePBIL();
    void solveGA();

private:
    // class variables
    vector < vector<int> > clauses;
    vector<double> PV;
    int individuals;
    double posRate;
    double negRate;
	double pC;
    double pM;
    double mutAmnt;
	string selection;
	string crossover;
    int generations;
    int numVariables;
    int numClauses;
	int bestValue;
	int generationFoundBest;
	int* best;
    int* fitnessList;
    int** population;
	int** breedingPool;

};


#endif
