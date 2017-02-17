#include "MAXSAT.h"
#include "math.h"

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
	for (int i = 0; i < individuals; i++)
		free(population[i]);
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

int compare ( const void *pa, const void *pb ) {
	const int *a = *(const int **)pa;
	const int *b = *(const int **)pb;

	return a[1] - b[1];
}

void MaxSat::arrayCopy(int* arr1, int* arr2, int size) {
	for (int i = 0; i < size; i++) {
		arr1[i] = arr2[i];
	}
}

void MaxSat::selectRanking() {
	//a 2-D array to store the index and fitness of each individual
	//this array will be sorted by fitness
	int** rankList = (int**) malloc(sizeof(int) * individuals  * 2);
	for (int i = 0; i < individuals; i++) {
		rankList[i] = (int*) malloc(sizeof(int) * 2);

		rankList[i][0] = i;
		rankList[i][1] = fitnessList[i];
	}

	//quicksort to sort the individuals by fitness
	qsort(rankList, individuals, sizeof rankList[0], compare);

	double sum = (individuals * (individuals + 1))/2;
	int i = 0;
	while (i < individuals) {
		for (int j = 0; j < individuals; j++) {
			double probability = (j+1)/(sum);

			//get a random num between 0 and 1. if that number is less
			//than the selection probability, select the individual
			double randNum = ((double) rand())/(RAND_MAX);

			if (randNum < probability) {
				int indexOfIndividual = rankList[j][0];
				arrayCopy(breedingPool[i], population[indexOfIndividual], numVariables);
				i++;
			}

			if (i >= individuals) {
				break;
			}
		}
	}

	free(rankList);
}

void MaxSat::selectTournament() {
	int randNum;

	int* individual1 = (int*) malloc(sizeof(int) * numVariables);
	int* individual2 = (int*) malloc(sizeof(int) * numVariables);
	for (int i = 0; i < individuals; i++) {
		randNum = rand() % individuals;
		arrayCopy(individual1, population[randNum], numVariables);
		int fitness1 = fitnessList[randNum];

		randNum = rand() % individuals;
		arrayCopy(individual2, population[randNum], numVariables);
		int fitness2 = fitnessList[randNum];

		if (fitness1 > fitness2) {
			arrayCopy(breedingPool[i], individual1, numVariables);
		} else {
			arrayCopy(breedingPool[i], individual2, numVariables);
		}
	}
	free(individual1);
	free(individual2);
}

void MaxSat::selectBoltzman() {
	int i = 0;

	double totalFitness = 0;
	for (int n = 0; n < individuals; n++) {
		totalFitness += exp(fitnessList[n]);
	}

	while (i < individuals) {
		for (int j = 0; j < individuals; j++) {
			//cout << "fitness of individual " << j << " is " << fitnessList[j] << endl;
			double probability = exp(fitnessList[j])/totalFitness;

			//get a random num between 0 and 1. if that number is less
			//than the selection probability, select the individual
			double randNum = ((double) rand())/(RAND_MAX);

			if (randNum < probability) {
				arrayCopy(breedingPool[i], population[j], numVariables);
				i++;
				/*cout << "selected individual with fitness " << fitnessList[j] << " and fitness prob " << probability << " above rand prob " << randNum << endl;*/
			}

			if (i >= individuals) {
				break;
			}
		}
	}
}
// NOTE: THIS WILL ONLY WORK FOR EVEN POPULATION I THINK
void MaxSat::onePCross() {

	for (int i = 0; i < individuals; i += 2) {
		int randNum = rand();
		if (randNum < pC){
			int* parent1 = breedingPool[i];
			int* parent2 = breedingPool[i+1];
			int crossPointRand = rand();
			int crossPoint = (int) (crossPointRand * numVariables);
			int* offspring1 = (int*) malloc(sizeof(int) * numVariables);
			int* offspring2 = (int*) malloc(sizeof(int) * numVariables);

			for (int j = 0; j < numVariables; j++) {
				if (j < crossPoint) {
					offspring1[j] = parent1[j];
					offspring2[j] = parent2[j];
				} else {
					offspring1[j] = parent2[j];
					offspring2[j] = parent1[j];
				}
			}
			arrayCopy(population[i], offspring1, numVariables);
			arrayCopy(population[i+1], offspring2, numVariables);
			free(offspring1);
			free(offspring2);
		} else {
			arrayCopy(population[i], breedingPool[i], numVariables);
			arrayCopy(population[i+1], breedingPool[i+1], numVariables);
		}
	}
}

void MaxSat::uniformCross() {
	for (int i = 0; i < individuals; i++) {
		int randNum = rand();
		if (randNum < pC) {
			int* parent1 = breedingPool[i];
			int* parent2;
			if (i+1 == individuals) {
				parent2 = breedingPool[0];
			} else {
				parent2 = breedingPool[i+1];
			}
			int* offspring = (int*) malloc(sizeof(int) * numVariables);

			for (int j = 0; j < numVariables; j++) {
				int pointProb = rand();
				if (pointProb < 0.5) {
					offspring[j] = parent1[j];
				} else {
					offspring[j] = parent2[j];
				}
			}
			arrayCopy(population[i], offspring, numVariables);
			free(offspring);
		} else {
			arrayCopy(population[i], breedingPool[i], numVariables);
		}
	}
}

void MaxSat::mutateOffspring() {
    int mutRand;
    for(int i = 0; i < individuals; i++) {
        for(int j = 0; j < numVariables; j++) {
            mutRand = rand() % 100;
            if((double) mutRand / 100 < pM) {
                // make mutation: reverse the value of the "bit"
                population[i][j] = !population[i][j];
            }
        }
    }
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

	breedingPool = (int**) malloc(sizeof(int) * individuals * numVariables);
	for(int i = 0; i < individuals; i++) {
		breedingPool[i] = (int*) malloc(sizeof(int) * numVariables);
	}
	
	for (int i = 0; i < generations; i++) {
		evalFitness();
        if(!selection.compare("rs")) {
            selectRanking();
        } else if(!selection.compare("ts")) {
            selectTournament();
        } else if(!selection.compare("bs")) {
            selectBoltzman();
        } else {
            cout << "error in selection: no valid selection method specified" << endl;
            exit(1);
        }

		/*cout << "Printing breeding pool (" << individuals << " individuals, " << numVariables << " size solution)..." << endl;
		for(int i = 0; i < individuals; i++) {
			printSolution(breedingPool[i]);
		}
		*/

        if(!crossover.compare("1c")) {
            onePCross();
        } else if(!crossover.compare("uc")) {
            uniformCross();
        } else {
            cout << "error in crossover: no valid crossover method specified" << endl;
            exit(1);
        }

		mutateOffspring();
		int bestFitness = findMaxFitness();
		if(i % (generations / 20) == 0) {
			// print most clauses satisfied 20 times, i.e. every generations/20 times.
			cout << "(Generation " << i << ") -- Best solution satisfied " << fitnessList[bestFitness] << " of " << clauses.size() << " clauses" << endl;
		}
	}
	
	cout << endl << endl;
	cout << "Best solution satisfied " << countSatClauses(population[findMaxFitness()]) << " of " << clauses.size() << " clauses" << endl;
	printSolution(population[findMaxFitness()]);

	free(breedingPool);
}

// ./main testcase-3-5-0.7-1.cnf 100 0.1 0.075 0.02 0.05 1000 p
// ./main testcase-3-5-0.7-1.cnf 100 ts 1c 0.7 0.01 1000 g

