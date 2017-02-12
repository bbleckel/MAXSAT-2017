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
    cout << "Printing population..." << endl;
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
    while(genRemaining >= 0) {
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
                cout << randNum << endl;
                cout << (double) randNum / 100 << endl;
                if((double) randNum / 100 < PV[j]) {
                    population[i][j] = 1;
                } else {
                    population[i][j] = 0;
                }
            }
        }
        // free after each iteration?
    }
    printPopulation(population, individuals, stringSize);
}
