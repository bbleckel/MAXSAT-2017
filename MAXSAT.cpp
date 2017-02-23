#include "MAXSAT.h"

using namespace std;

MaxSat::MaxSat(string fileName, int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations) {
    // PBIL constructor

    readFile(fileName);

    this->individuals = individuals;
	this->posRate = posRate;
	this->negRate = negRate;
	this->pM = pM;
	this->mutAmnt = mutAmnt;
	this->generations = generations;

	// allocate array memory
	fitnessList = (int*) malloc(sizeof(int) * individuals);
	population = (int**) malloc(sizeof(int) * individuals * numVariables);
	for(int i = 0; i < individuals; i++) {
		population[i] = (int*) malloc(sizeof(int) * numVariables);
	}
}

MaxSat::MaxSat(string fileName, int individuals, string selection, string crossover, double pC, double pM, int generations) {
    // GA constructor

    readFile(fileName);

	this->individuals = individuals;
	this->selection = selection;
	this->crossover = crossover;
	this->pC = pC;
	this->pM = pM;
	this->generations = generations;
	this->bestValue = 0;

    breedingPool = (int**) malloc(sizeof(int) * individuals * numVariables);

    for(int i = 0; i < individuals; i++) {
        breedingPool[i] = (int*) malloc(sizeof(int) * numVariables);
    }

	fitnessList = (int*) malloc(sizeof(int) * individuals);
	population = (int**) malloc(sizeof(int) * individuals * numVariables);
	for(int i = 0; i < individuals; i++) {
		population[i] = (int*) malloc(sizeof(int) * numVariables);
	}
}

MaxSat::~MaxSat() {
	// free malloc'ed variables
	free(fitnessList);
	for (int i = 0; i < individuals; i++)
		free(population[i]);
	free(population);
}

void MaxSat::readFile(string fileName) {
    // read file
    string line;
    ifstream inputFile;
    inputFile.open(fileName, ios::in);
    if(!inputFile.is_open()) {
        cerr << "ERROR: Could not open file" << endl;
        exit(1);
    } else {
        while(getline(inputFile, line)) {
            if (line.front() == 'c') {
                // line is a comment (starts with "c"), should not be included
                // (for some reason, need this "if" before the other checks)

            } else if (line.front() == 'p') {
                // the line just before the data begins
                // contains information about the data if we want it
                string entry;
                string delimiter = " ";
                // get rid of "p" and "cnf"
                entry = line.substr(0, line.find(delimiter));
                line.erase(0, line.find(delimiter) + delimiter.length());
                entry = line.substr(0, line.find(delimiter));
                line.erase(0, line.find(delimiter) + delimiter.length());

                // save number of variables & clauses
                numVariables = stoi(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());
                numClauses = stoi(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

            } else if (line.back() == '0'){
                // line should be included in data we are using
                string entry;
                string delimiter = " ";
                vector<int> clause;
                while((entry = line.substr(0, line.find(delimiter))) != "0") {
                    clause.push_back(stoi(entry));
                    line.erase(0, line.find(delimiter) + delimiter.length());
                }
                clauses.push_back(clause);
            }
        }
        inputFile.close();
    }
}

void MaxSat::initPV() {
	PV.clear();
	// initialize PV with 0.5
	for(int i = 0; i < numVariables; i++) {
		PV.push_back(0.5);
	}
}

void MaxSat::initPopulation() {
	//initialize population for GA with each individual
	//holding a random value of 1 or 0
	int randNum;
	for (int i = 0; i < individuals; i++) {
		for (int j = 0; j < numVariables; j++) {
			randNum = rand() % 2;
			population[i][j] = randNum;
		}
	}
}

int MaxSat::countSatClauses(int* solution) {

	int count = 0;
	// each solution requires O(numClauses * numVariables) to count satisfied clauses

	//iterate through clauses
	for(int i = 0; i < clauses.size(); i++) {
		// iterate through variables of each clause
		for(int j = 0; j < clauses[0].size(); j++) {
			/* solution value of 0 and negative variable in clause or
			 solution value of 1 and positive variable in clause */
			int value;
			if(clauses[i][j] < 0) {
                // take positive value
				value = -1 * clauses[i][j];

				// note: should never be a variable 0, as 0's are reserved for end of line
                // since the variable is negative, we'd like there to be a 0 in solution
				if(!solution[value - 1]) {
					count++;
					break;
				}
			} else {
				value = clauses[i][j];

                // since variable is positive, we want a 1 in solution
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
    // find index of max value in fitnessList
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
    // find index of min value in fitnessList
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

int compare (const void *pa, const void *pb) {
    const int *a = *(const int **)pa;
    const int *b = *(const int **)pb;

    return a[1] - b[1];
}

//array copy is used to deal with moving malloced arrays
void MaxSat::arrayCopy(int* arr1, int* arr2, int size) {
	for (int i = 0; i < size; i++) {
		arr1[i] = arr2[i];
	}
}

void MaxSat::selectRanking() {
	//a 2-D array to store the index and fitness of each individual
	//this array will be sorted by fitness
	//this is useful to keep track of the index of the individual
	//in the population list after we sort
	int** rankList = (int**) malloc(sizeof(int) * individuals  * 2);
	for (int i = 0; i < individuals; i++) {
		rankList[i] = (int*) malloc(sizeof(int) * 2);

		//save index
		rankList[i][0] = i;
		//save fitness
		rankList[i][1] = fitnessList[i];
	}

	//quicksort to sort the individuals by fitness
	qsort(rankList, individuals, sizeof rankList[0], compare);

	//calcualte total sum of individuals for denominator
	//of ranksort formula
	double sum = (individuals * (individuals + 1))/2;

	//outer loop to add n new individuals to the breeding pool
	for (int i = 0; i < individuals; i++) {
		double probability = 0;
		//generate a random probability
		double randomProbability = ((double) rand())/(INT_MAX);
		//sum up probability of individuals in ranked order until the
		//cumulative probability is greater than the random probability
		//generated
		for (int j = 0; j < individuals; j++) {
			probability += (j+1)/(sum);

			//once the sum is greater than or equal to the random probability
			//add the individual that broke the barrier of the random prob
			if (probability >= randomProbability) {
				arrayCopy(breedingPool[i], population[rankList[j][0]], numVariables);
			}
		}
	}

	//free pointer
	free(rankList);
}

void MaxSat::selectTournament() {
	int randNum;
	//malloc for two indiviuals
	int* individual1 = (int*) malloc(sizeof(int) * numVariables);
	int* individual2 = (int*) malloc(sizeof(int) * numVariables);
	//select n = individuals individuals
	for (int i = 0; i < individuals; i++) {
		//generate a random index and set population at that
		//index to be the first individual
		randNum = rand() % individuals;
		arrayCopy(individual1, population[randNum], numVariables);
		//get fitness of individual
		int fitness1 = fitnessList[randNum];

		//generate another random index and set population at it
		//to second individual
		randNum = rand() % individuals;
		arrayCopy(individual2, population[randNum], numVariables);
		//get the fitness
		int fitness2 = fitnessList[randNum];

		//compare fitnesses; put more fit one
		//into breeding pool
		if (fitness1 > fitness2) {
			arrayCopy(breedingPool[i], individual1, numVariables);
		} else {
			arrayCopy(breedingPool[i], individual2, numVariables);
		}
	}

	//free pointers
	free(individual1);
	free(individual2);
}

void MaxSat::selectBoltzmann() {
	//calcualte the total fitness for denominator of
	//boltzman formula. this is the sum of
	//e^fitness of each individual
	double totalFitness = 0;
	for (int n = 0; n < individuals; n++) {
		totalFitness += exp(fitnessList[n]);
	}

	//select n=individuals individuals
	for (int i = 0; i < individuals; i++) {
		double probability = 0;
		//generate a random probability 0<->1
		double randomProbability = ((double) rand())/(INT_MAX);
		//sum up probabilities of individuals until their cumulative
		//probability is greater than or equal to the random one
		//generated
		for (int j = 0; j < individuals; j++) {
			//probability of each individual is defined by
			//e^fitness of the individual all divided by the
			//previously calcualted total finess
			probability += exp(fitnessList[j])/totalFitness;

			//once the probability is high enough, add the individual
			//to the breeding pool
			if (probability >= randomProbability) {
				arrayCopy(breedingPool[i], population[j], numVariables);
			}
		}
	}
}

// function to perform one point crossover
void MaxSat::onePCross() {
    // allocate the offspring outside the loop so it only has to be done once
    int* offspring1 = (int*) malloc(sizeof(int) * numVariables);
    int* offspring2 = (int*) malloc(sizeof(int) * numVariables);
    // looking at individuals in pairs so looping through them by twos
    for (int i = 0; i < individuals - 1; i += 2) {

        // only performs crossover with a probability defined by the user
        double randNum = ((double) rand())/(RAND_MAX);
        if (randNum < pC){
            // first parent is current location in breeding pool
            int* parent1 = breedingPool[i];
            // second parent is next in breeding pool or loops back to the start
            // if parent1 is the last one in the breeding pool
            int* parent2;
            if (i+1 >= individuals) {
                parent2 = breedingPool[0];
            } else {
                parent2 = breedingPool[i+1];
            }

            // set the cross point to be a random location
            double crossPointRand = ((double) rand())/(RAND_MAX);
            int crossPoint = (int) (crossPointRand * numVariables);

            // loop through to create the children
            for (int j = 0; j < numVariables; j++) {
                // before the crosspoint
                if (j < crossPoint) {
                    offspring1[j] = parent1[j];
                    offspring2[j] = parent2[j];

                // after the crosspoint
                } else {
                    offspring1[j] = parent2[j];
                    offspring2[j] = parent1[j];
                }
            }

            // copy the new offspring into the population
            arrayCopy(population[i], offspring1, numVariables);
            arrayCopy(population[i+1], offspring2, numVariables);

        // crossover did not happen, move the parents right into the population
        } else {
            arrayCopy(population[i], breedingPool[i], numVariables);
            arrayCopy(population[i+1], breedingPool[i+1], numVariables);
        }
    }
    // free the offspring
    free(offspring1);
    free(offspring2);
}

// function to perform uniform crossover
void MaxSat::uniformCross() {
    // allocate the offspring outside the loop so it only has to be done once
    int* offspring = (int*) malloc(sizeof(int) * numVariables);
    // looking at individuals one at a time in the breeding pool
    for (int i = 0; i < individuals; i++) {
        // does crossover with a probability defined by the user
        double randNum = ((double) rand())/(RAND_MAX);
        if (randNum < pC) {
            // parent1 is the current location in breeding pool
            int* parent1 = breedingPool[i];
            int* parent2;
            // parent2 is the next individual unless parent1 is the last
            // individual, then parent2 is the first individual
            if (i+1 >= individuals) {
                parent2 = breedingPool[0];
            } else {
                parent2 = breedingPool[i+1];
            }

            // loops through each index in the offspring
            for (int j = 0; j < numVariables; j++) {
                // chooses from the parents with equal probability
                double pointProb = ((double) rand())/(RAND_MAX);
                if (pointProb < 0.5) {
                    offspring[j] = parent1[j];
                } else {
                    offspring[j] = parent2[j];
                }
            }
            // copy the offspring into the population
            arrayCopy(population[i], offspring, numVariables);
        // crossover did not happen, move parent1 right into the population
        } else {
            arrayCopy(population[i], breedingPool[i], numVariables);
        }
    }
    // free the offspring
    free(offspring);
}

void MaxSat::mutatePV() {
    double mutRand;
    int dirRand;
    int mutDirection;
    for(int i = 0; i < PV.size() - 1; i++) {
        mutRand = ((double) rand())/(RAND_MAX);
        // mutate or not?
        if(mutRand < pM) {
            dirRand = rand() % 100;
            // choose mutation direction
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
	srand(time(NULL));

	// initialize PV
	initPV();

    double randNum;
    // keeps track of the most clauses satisfied over the whole process
    bestValue = 0;

	int genRemaining = generations;

	while(genRemaining >= 0) {
		// create population
		for(int i = 0; i < individuals; i++) {
			for(int j = 0; j < numVariables; j++) {
				randNum = ((double) rand())/(INT_MAX);
				if(randNum < PV[j]) {
					population[i][j] = 1;
				} else {
					population[i][j] = 0;
				}
			}
		}
        // evaluate fitness of generated population
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

        if(fitnessList[bestFitness] > bestValue) {
            bestValue = fitnessList[bestFitness];
            genFound = generations - genRemaining;
        }

		genRemaining--;
	}
}

void MaxSat::solveGA() {
    double bestList[4] = {168.0, 238.0, 2.0, 40.0};

    srand(time(NULL));
<<<<<<< HEAD

	initPopulation();

=======

	//initialize population
	initPopulation();

	//best and breedinPool are class variables
	//malloc space for best
>>>>>>> origin/master
    best = (int*) malloc(sizeof(int) * numVariables);
	//malloc space for breeding pool (2-D array)
	breedingPool = (int**) malloc(sizeof(int) * individuals * numVariables);
	for(int i = 0; i < individuals; i++) {
		breedingPool[i] = (int*) malloc(sizeof(int) * numVariables);
	}

	//iterate for total number of generations
	for (int i = 0; i < generations; i++) {
<<<<<<< HEAD

        // evaluate fitness first
		evalFitness();

=======
		//first evaluate fitness
		evalFitness();

		//apply selection to get breeding pool based on user's
		//choice for selection method
>>>>>>> origin/master
        // create new individuals through selection
		if(!selection.compare("rs")) {
			selectRanking();
		} else if(!selection.compare("ts")) {
			selectTournament();
		} else if(!selection.compare("bs")) {
			selectBoltzmann();
		}

		//apply crossover to replace population. used method
		//for crossover based on user input on command line
		if(!crossover.compare("1c")) {
			onePCross();
		} else if(!crossover.compare("uc")) {
			uniformCross();
		}

        // perform mutation on population
		mutateOffspring();

        // evaluate fitness again
		evalFitness();

		//save the best fitness as well as the most fit individual
		//everytime you run a generation. this makes it so that we
		//can print our the best individual found at the end of our
		//algorithm, thus giving us the most fit possible individual
		int bestFitness = findMaxFitness();
		if (fitnessList[bestFitness] > bestValue) {
            // update working best
			genFound = i + 1;
			bestValue = fitnessList[bestFitness];
			arrayCopy(best, population[bestFitness], numVariables);
		}
        if((generations - i) % (generations / 20) == 0) {
            // print current solution each 20th of total generations
            cout << "(Generation " << i << ") -- Best solution satisfied " << fitnessList[bestFitness] << " of " << clauses.size() << " clauses" << endl;
        }
	}
	free(breedingPool);
}
