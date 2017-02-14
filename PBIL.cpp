#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>


// possible improvement: rewrite to have PBIL class

using namespace std;

void printSolution(int* solution, int stringSize) {
    cout << "[";
    for(int i = 0; i < stringSize - 1; i++) {
        cout << solution[i] << ", ";
    }
    cout << solution[stringSize - 1] << "]" << endl;
}

void printPopulation(int** population, int size, int stringSize) {
    cout << "Printing population (" << size << " individuals, " << stringSize << " size solution)..." << endl;
    for(int i = 0; i < size; i++) {
        printSolution(population[i], stringSize);
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
    srand(time(NULL));
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
    for(int i = 0; i < clauses.size(); i++) {
        for(int j = 0; j < clauses[0].size(); j++) {
            /* solution value of 0 and negative variable in clause or
             solution value of 1 and positive variable in clause */
            if(clauses[i][j] < 0) {
                cout << "Clause " << i << " has variable " << j << " negative!" << endl;
            }
        }
    }
    
    return count;
}

int* evalFitness(vector< vector<int> > clauses, int** population, int size, int stringSize) {
    int* fitnessList = (int*) malloc(sizeof(int) * size);
    int fitness;
    
    srand(time(NULL));
    // iterate through solutions
    for(int i = 0; i < size; i++) {
        // evaluate fitness of each candidate solution
        // (temporarily random)
        countSatClauses(clauses, population[i]);
        fitness = rand() % 100;
        fitnessList[i] = fitness;
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

void PBIL_MAXSAT(vector< vector<int> > clauses, int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations, int stringSize) {
    cout << "Solving with PBIL..." << endl;
    
    vector<double> PV;
    
    // initialize PV
    for(int i = 0; i < stringSize; i++) {
        PV.push_back(0.5);
    }
    
    int randNum;
    int** population = (int**) malloc(sizeof(int) * individuals * stringSize);
    for(int i = 0; i < individuals; i++) {
        population[i] = (int*) malloc(sizeof(int) * stringSize);
    }
    int genRemaining = generations;
    while(genRemaining >= 0) {
        
        // create population
        srand(time(NULL));
        for(int i = 0; i < individuals; i++) {
            for(int j = 0; j < stringSize; j++) {
                randNum = rand() % 100;
                if((double) randNum / 100 < PV[j]) {
                    population[i][j] = 1;
                } else {
                    population[i][j] = 0;
                }
            }
        }
        
        int* fitnessList = evalFitness(clauses, population, individuals, stringSize);
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
        
//        cout << "Printing best then worst solutions..." << endl;
//        printSolution(population[bestFitness], stringSize);
//        printSolution(population[worstFitness], stringSize);
        
        genRemaining--;
    }
    printPopulation(population, individuals, stringSize);
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
