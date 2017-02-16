#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

/*
 * Genetic Algorithm Implementation
 *
 * CS3445 - Nature Inspired Computation
 *
 * Dylan Parsons, Jasper Houston, Bo Bleckel
 *
 */

using namespace std;

vector< vector< int > > generateRandomPopulation(int individuals, int numVariables) {
	vector< vector< int > > randPopulationVect;
	
	int randNum;
	for (int i = 0; i < individuals; i++) {
		for (int j = 0; i < numVariables; j++) {
			randNum = rand() % 2;
			randPopulationVect[i].push_back(randNum);
		}
	}
	
	return randPopulationVect;
}

int countSatClauses(vector< vector<int> > clauses, vector< vector< int > > solution) {
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
				if(!solution.at(value - 1)) {
					count++;
					break;
				}
			} else {
				value = clauses[i][j];
				
				if(solution.at(value - 1)) {
					count++;
					break;
				}
			}
		}
	}
	
	return count;
}

int* evalFitness(vector< vector<int> > clauses, vector< vector < int > population, int size, int numVariables) {
	int* fitnessList = (int*) malloc(sizeof(int) * size);
	int fitness;
	
	// iterate through solutions
	for(int i = 0; i < size; i++) {
		// evaluate fitness of each candidate solution
		fitnessList[i] = countSatClauses(clauses, population[i]);
	}
	return fitnessList;
}

void GA_MAXSAT(vector< vector< int > > clauses, int individuals, string selectionMethod, string crossOverMethod, double pC, double pM, int generations, int numVariables) {
	cout << "Solving with GA..." << endl;
	
	srand(time(NULL));

	vector< vector< int > > population = generateRandomPopulation(individuals, numVariables);
	
	for (int i = 0; i < generations; i++) {
		//select(clauses, selectionMethod, individuals);
		
		//mate();
		
		//mutate();
	}
}
