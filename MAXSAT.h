#ifndef MAXSAT_H
#define MAXSAT_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

class MaxSat {

public:
    // constructor/destructor
    MaxSat(vector< vector<int> > clauses, int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations, int numVariables);
    ~MaxSat();
    void initPV();
    
    // fitness
    int countSatClauses(int* solution);
    void evalFitness();
    int findMaxFitness();
    int findMinFitness();
    
    // mutation
    void mutatePV();
    
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
    double pM;
    double mutAmnt;
    int generations;
    int numVariables;
    int* fitnessList;
    int** population;

};


#endif