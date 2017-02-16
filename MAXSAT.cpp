#include "MAXSAT.h"

using namespace std;

MaxSat::MaxSat(vector< vector<int> > clauses, int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations, int numVariables) {
	this->clauses = clauses;
	this->individuals = individuals;
	this->posRate = posRate;
	this->negRate = negRate;
	this->pM = pM;
	this->mutAmnt = mutAmnt;
	this->generations = generations;
	this->numVariables = numVariables;
	
	// allocate array memory
	fitnessList = (int*) malloc(sizeof(int) * individuals);
	population = (int**) malloc(sizeof(int) * individuals * numVariables);
	for(int i = 0; i < individuals; i++) {
		population[i] = (int*) malloc(sizeof(int) * numVariables);
	}
	cout << "Created class for PBIL" << endl;
	
}

MaxSat::MaxSat(vector< vector<int> > clauses, int individuals, string selection, string crossover, double pC, double pM, int generations, int numVariables) {
	this->clauses = clauses;
	this->individuals = individuals;
	this->selection = selection;
	this->crossover = crossover;
	this->pC = pC;
	this->pM = pM;
	this->generations = generations;
	this->numVariables = numVariables;
	
	fitnessList = (int*) malloc(sizeof(int) * individuals);
	population = (int**) malloc(sizeof(int) * individuals * numVariables);
	for(int i = 0; i < individuals; i++) {
		population[i] = (int*) malloc(sizeof(int) * numVariables);
	}
	cout << "Created class for GA" << endl;
	
}

MaxSat::~MaxSat() {
	// free malloc'ed variables
	free(fitnessList);
	free(population);
	// delete vectors?
	cout << "Freeing MaxSat variables. Goodbye!" << endl;
}

void MaxSat::initPV() {
	PV.clear();
	// initialize PV
	for(int i = 0; i < numVariables; i++) {
		PV.push_back(0.5);
	}
}

int MaxSat::countSatClauses(int* solution) {
	int count = 0;
	// each solution requires O(numClauses * numVariables) to count satisfied clauses
	
	//iterate through clauses
	for(int i = 0; i < clauses.size(); i++) {
		// iterate through variables of each clase
		for(int j = 0; j < clauses[0].size(); j++) {
			/* solution value of 0 and negative variable in clause or
			 solution value of 1 and positive variable in clause */
			int value;
			if(clauses[i][j] < 0) {
				value = -1 * clauses[i][j];
				
				// note: should never be a variable 0, as 0's are reserved for end of line
				if(!solution[value - 1]) {
					count++;
					break;
				}
			} else {
				value = clauses[i][j];
				
				if(solution[value - 1]) {
					count++;
					break;
				}
			}
		}
	}
	
	return count;
}

void MaxSat::evalFitness() {
	// iterate through solutions
	for(int i = 0; i < individuals; i++) {
		// evaluate fitness of each candidate solution
		fitnessList[i] = countSatClauses(population[i]);
	}
}

int MaxSat::findMaxFitness() {
	int maxIndex = 0;
	int max = fitnessList[0];
	for(int i = 0; i < individuals; i++) {
		if(fitnessList[i] > max) {
			max = fitnessList[i];
			maxIndex = i;
		}
	}
	return maxIndex;
}

int MaxSat::findMinFitness() {
	int minIndex = 0;
	int min = fitnessList[0];
	for(int i = 0; i < individuals; i++) {
		if(fitnessList[i] < min) {
			min = fitnessList[i];
			minIndex = i;
		}
	}
	return minIndex;
}

void MaxSat::mutatePV() {
	int mutRand;
	int dirRand;
	int mutDirection;
	for(int i = 0; i < PV.size() - 1; i++) {
		mutRand = rand() % 100;
		if((double) mutRand / 100 < pM) {
			dirRand = rand() % 100;
			if((double) dirRand / 100 < 0.5) {
				mutDirection = 1;
			} else {
				mutDirection = 0;
			}
			// make mutation
			PV[i] = PV[i] * (1.0 - mutAmnt) + mutDirection * mutAmnt;
		}
	}
}

void MaxSat::printSolution(int* solution) {
	cout << "[";
	for(int i = 0; i < numVariables - 1; i++) {
		cout << solution[i] << ", ";
	}
	cout << solution[numVariables - 1] << "]" << endl;
}

void MaxSat::printPopulation() {
	cout << "Printing population (" << individuals << " individuals, " << numVariables << " size solution)..." << endl;
	for(int i = 0; i < individuals; i++) {
		printSolution(population[i]);
	}
}

void MaxSat::printPV() {
	cout << "Printing current population vector..." << endl;
	cout << "[";
	for(int i = 0; i < PV.size() - 1; i++) {
		cout << PV[i] << ", ";
	}
	cout << PV[PV.size() - 1] << "]" << endl;
}

void MaxSat::printClauses() {
	cout << "Printing clauses:" << endl;
	for(int i = 0; i < clauses.size(); i++) {
		for(int j = 0; j < clauses[0].size(); j++) {
			// is it possible for clauses[0].size() to cause error in extreme cases?
			cout << clauses[i].at(j) << ", ";
		}
		cout << "\n";
	}
}

void MaxSat::solvePBIL() {
	cout << "Solving with PBIL..." << endl;
	
	srand(time(NULL));
	
	// initialize PV
	initPV();
	
	int randNum;
	
	int genRemaining = generations;
	
	while(genRemaining >= 0) {
		// create population
		for(int i = 0; i < individuals; i++) {
			for(int j = 0; j < numVariables; j++) {
				randNum = rand() % 100;
				if((double) randNum / 100 < PV[j]) {
					population[i][j] = 1;
				} else {
					population[i][j] = 0;
				}
			}
		}
		
		evalFitness();
		int bestFitness = findMaxFitness();
		int worstFitness = findMinFitness();
		
		// update PV towards best solution
		for(int i = 0; i < PV.size(); i++) {
			PV[i] = PV[i] * (1.0 - posRate) + population[bestFitness][i] * posRate;
		}
		// update PV away from worst solution
		for(int i = 0; i < PV.size(); i++) {
			if(population[bestFitness][i] != population[worstFitness][i]) {
				// only change the values that differ from the best solution's values
				PV[i] = PV[i] * (1.0 - negRate) + population[bestFitness][i] * negRate;
			}
		}
		
		// mutate!
		mutatePV();
		
		genRemaining--;
		if(genRemaining % (generations / 20) == 0) {
			// print most clauses satisfied each 10 generations
			cout << "(Generation " << generations - genRemaining << ") -- Best solution satisfied " << fitnessList[bestFitness] << " of " << clauses.size() << " clauses" << endl;
		}
	}
	
	// for test purposes, show archetype solution
	int* sol = (int*) malloc(sizeof(int) * numVariables);
	for(int i = 0; i < numVariables; i++) {
		if(PV[i] > 0.5) {
			sol[i] = 1;
		} else {
			sol[i] = 0;
		}
	}
	
	
	
	cout << "Best solution satisfied " << countSatClauses(population[findMaxFitness()]) << " of " << clauses.size() << " clauses" << endl;
	cout << "Archetypical solution satisfied " << countSatClauses(sol) << " of " << clauses.size() << " clauses:" << endl;
	printSolution(sol);
	
	
	//    printPopulation(population, individuals, numVariables);
	printPV();
	
	// to free: fitnessList, population, PV
}

void MaxSat::selectRanking() {
	
}

void MaxSat::selectTournament() {
	
}

void MaxSat::selectBoltzman() {
	
}

void MaxSat::onePCross() {
	
}

void MaxSat::uniformCross() {
	
}

void MaxSat::mutateOffspring() {
	
}

void MaxSat::initPopulation() {
	int randNum;
	for (int i = 0; i < individuals; i++) {
		for (int j = 0; j < numVariables; j++) {
			randNum = rand() % 2;
			population[i][j] = randNum;
		}
	}
}

void MaxSat::solveGA() {
	cout << "Solving with GA..." << endl;
	
	srand(time(NULL));
	
	initPopulation();
	
	for (int i = 0; i < generations; i++) {
		evalFitness();
		
		int maxFit = findMaxFitness();
		int minFit = findMinFitness();
		
		switch (selection) {
			case "rs":
				selectRanking();
				break;
			case "ts":
				selectTournament();
				break;
			case "bs":
				selectBoltzman();
				break;
			default:
				cout << "error in selection: no valid selection method specified" << endl;
				exit();
		}
		
		switch (crossover) {
			case "1c":
				onePCross();
				break;
			case "uc":
				uniformCross();
				break;
			default:
				cout << "error in crossover: no valid crossover method specified" << endl;
				exit();
		}
		
		mutateOffspring();
	}
}
