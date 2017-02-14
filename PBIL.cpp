#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>


// possible improvement: rewrite to have PBIL class

using namespace std;

void printSolution(int* solution, int numVariables) {
    cout << "[";
    for(int i = 0; i < numVariables - 1; i++) {
        cout << solution[i] << ", ";
    }
    cout << solution[numVariables - 1] << "]" << endl;
}

void printPopulation(int** population, int size, int numVariables) {
    cout << "Printing population (" << size << " individuals, " << numVariables << " size solution)..." << endl;
    for(int i = 0; i < size; i++) {
        printSolution(population[i], numVariables);
    }
}

void printPV(vector<double> PV) {
    cout << "Printing current population vector..." << endl;
    cout << "[";
    for(int i = 0; i < PV.size() - 1; i++) {
        cout << PV[i] << ", ";
    }
    cout << PV[PV.size() - 1] << "]" << endl;
}

void mutatePV(vector<double> &PV, double pM, double mutAmnt) {
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

int countSatClauses(vector< vector<int> > clauses, int* solution) {
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

int* evalFitness(vector< vector<int> > clauses, int** population, int size, int numVariables) {
    int* fitnessList = (int*) malloc(sizeof(int) * size);
    int fitness;
    
    // iterate through solutions
    for(int i = 0; i < size; i++) {
        // evaluate fitness of each candidate solution
        fitnessList[i] = countSatClauses(clauses, population[i]);
    }
    return fitnessList;
}

int findMaxFitness(int* fitnessList, int size) {
    int maxIndex = 0;
    int max = fitnessList[0];
    for(int i = 0; i < size; i++) {
        if(fitnessList[i] > max) {
            max = fitnessList[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}

int findMinFitness(int* fitnessList, int size) {
    int minIndex = 0;
    int min = fitnessList[0];
    for(int i = 0; i < size; i++) {
        if(fitnessList[i] < min) {
            min = fitnessList[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void PBIL_MAXSAT(vector< vector<int> > clauses, int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations, int numVariables) {
    cout << "Solving with PBIL..." << endl;
    
    vector<double> PV;
    
    srand(time(NULL));

    
    // initialize PV
    for(int i = 0; i < numVariables; i++) {
        PV.push_back(0.5);
    }
    
    int randNum;
    int** population = (int**) malloc(sizeof(int) * individuals * numVariables);
    for(int i = 0; i < individuals; i++) {
        population[i] = (int*) malloc(sizeof(int) * numVariables);
    }
    int genRemaining = generations;

    int* fitnessList;
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

        fitnessList = evalFitness(clauses, population, individuals, numVariables);
        int bestFitness = findMaxFitness(fitnessList, individuals);
        int worstFitness = findMinFitness(fitnessList, individuals);

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
        mutatePV(PV, pM, mutAmnt);
        
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
    
    
    
    cout << "Best solution satisfied " << countSatClauses(clauses, population[findMaxFitness(fitnessList, individuals)]) << " of " << clauses.size() << " clauses" << endl;
    cout << "Archetypical solution satisfied " << countSatClauses(clauses, sol) << " of " << clauses.size() << " clauses:" << endl;
    printSolution(sol, numVariables);

    
//    printPopulation(population, individuals, numVariables);
    printPV(PV);
    
    // to free: fitnessList, population, PV
}




// example of reading file
/*
 void readFile(char *fileName, lineType *cacheArray, int vFlag, int sValue, int EValue, int bValue) {
 // fgets implementation from tutorialspoint.com
 
 FILE *fp;
 // line is argument of sscanf, contains raw input from file
 char *line;
 // operation, address, and size are from parsed line
 char *operation;
 uint64_t address = 0;
 int size = 0;
 int count = 0;
 int tagMask, indexValue, tagValue;
 
 // will keep track of hits, misses, and evicts
 infoType *info = createInfo();
 info->hit = 0;
 info->miss = 0;
 info->evict = 0;
 
 fp = fopen(fileName, "r");
 if(fp == NULL)
 {
 perror("Error opening file");
 return;
 }
 
 line = (char *)malloc(sizeof(char*));
 operation = (char *)malloc(sizeof(char*));
 
 // the "sizeof(line) * 2" seems to cause problems without *2: runs twice
 while(fgets(line, sizeof(line) * 2, fp) != NULL) {
 // count is the basis of the LRU process
 count++;
 sscanf(line, "%s %lx,%d", operation, &address, &size);
 if(vFlag)
 printf("%s %lx,%d ", operation, address, size);
 
 // Get info from address
 indexValue = (address<<(64-sValue-bValue))>>(64-sValue);
 
 tagMask = (-1<<(bValue + sValue));
 tagValue = (address & tagMask)>>(bValue + sValue);
 
 // gives info from the address to a function that will complete the appropriate operations on the cache
 accessCache(cacheArray, vFlag, EValue, indexValue, tagValue, operation, count, info);
 if(vFlag)
 printf("\n");
 }
 fclose(fp);
 
 printSummary(info->hit, info->miss, info->evict);
 }
 */
