#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

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

void PBIL_MAXSAT(int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations, int stringSize) {
    cout << "Solving with PBIL..." << endl;
    
    vector<double> PV;
    
    // initialize PV
    for(int i = 0; i < stringSize; i++) {
        PV.push_back(0.5);
    }
    
    int genRemaining = generations;
//    while(genRemaining >= 0) {
        // create population
        int randNum;
        int** population = (int**) malloc(sizeof(int) * individuals * stringSize);
        for(int i = 0; i < individuals; i++) {
            population[i] = (int*) malloc(sizeof(int) * stringSize);
        }
        
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
        // free after each iteration?
//    }
    printPopulation(population, individuals, stringSize);
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
