// these were the includes before; since MAXSAT.h is included and has all these, shouldn't need them
//#include <cstdlib>
//#include <iostream>
//#include <ctime>
//#include <unistd.h>
//#include <string.h>
//#include <fstream>
//#include "MAXSAT.h"

#include <unistd.h>
#include "MAXSAT.h"

using namespace std;

void printInfo() {
    cout << endl;
    cout << "Usage:\nFor Genetic Algorithms:\n./main file_name individuals selection crossover pC pM generations algorithm" << endl;
    cout << "    file_name    = name of file from which to read (string)" << endl;
    cout << "    individuals  = number of individuals in population (int)" << endl;
    cout << "    selection    = type of selection of breeding pool (string):" << endl;
    cout << "                     ts   = tournament selection - implies ts1" << endl;
    cout << "                            ts1 = same individual cannot compete against self" << endl;
    cout << "                            ts2 = same individual can compete against self" << endl;
    cout << "                     rs   = rank-based selection" << endl;
    cout << "                     bs   = Boltzmann selection" << endl;
    cout << "    crossover    = crossover method (string):" << endl;
    cout << "                     1c   = 1-point crossover" << endl;
    cout << "                     uc   = uniform crossover" << endl;
    cout << "    pC           = crossover probability (double)" << endl;
    cout << "    pM           = mutation probability (double)" << endl;
    cout << "    generations  = max number of generations to run (int)" << endl;
    cout << "    algorithm    = type of algorithm (g or p) (string)" << endl;
    cout << "\nFor PBIL:\n./main file_name individuals pos_learning_rate neg_learning_rate mutation_prob mutation_amnt generations algorithm" << endl;
    cout << "    file_name          = name of file from which to read (string)" << endl;
    cout << "    individuals        = number of individuals in population (int)" << endl;
    cout << "    pos_learning_rate  = positive learning rate for best-individual update (double):" << endl;
    cout << "    neg_learning_rate  = negative learning rate for worst-individual update (double):" << endl;
    cout << "    mutation_prob      = mutation probability (double)" << endl;
    cout << "    mutation_amnt      = amount the PV is changed if mutated (double)" << endl;
    cout << "    generations        = max number of generations to run (int)" << endl;
    cout << "    algorithm          = type of algorithm (g or p) (string)" << endl;
    cout << endl;
}

void testCases() {
//    int popSizes[5] = {5, 10, 100, 500, 1000};
//    int genSizes[5] = {100, 500, 1000, 2000, 10000};
//    double posRate[3] = {0.01, 0.1, 0.75};
//    double negRate[3] = {0.01, 0.1, 0.75};
//    double pMList[5] = {0.001, 0.01, 0.1, 0.5, 1.0};
//    double amntList[5] = {0.005, 0.05, 0.25, 0.5, 1.0};
//    
//    string fileName[11] = {"../maxsat-problems/maxsat-crafted/bipartite/maxcut-140-630-0.7/maxcut-140-630-0.7-8.cnf", "../maxsat-problems/maxsat-crafted/bipartite/maxcut-140-630-0.7/maxcut-140-630-0.7-39.cnf", "../maxsat-problems/maxsat-crafted/bipartite/maxcut-140-630-0.8/maxcut-140-630-0.8-11.cnf", "../maxsat-problems/maxsat-crafted/MAXCUT/DIMACS_MOD/brock200_4.clq.cnf", "../maxsat-problems/maxsat-crafted/MAXCUT/DIMACS_MOD/MANN_a27.clq.cnf", "../maxsat-problems/maxsat-crafted/MAXCUT/SPINGLASS/t5pm3-7777.spn.cnf", "../maxsat-problems/maxsat-random/highgirth/4SAT/HG-4SAT-V100-C900-23.cnf", "../maxsat-problems/maxsat-random/max2sat/120v/s2v120c1200-10.cnf", "../maxsat-problems/maxsat-random/max2sat/140v/s2v140c1600-6.cnf", "../maxsat-problems/maxsat-random/max3sat/60v/s3v60c900-4.cnf", "../maxsat-problems/maxsat-random/max3sat/80v/s3v80c1000-2.cnf"};
    
//    int popSizes[3] = {10, 100, 1000};
//    int genSizes[3] = {100, 2000, 10000};
    int popSizes[3] = {10, 10, 10};
    int genSizes[3] = {10, 100, 10};
    double posRate[2] = {0.1, 0.75};
    double negRate[2] = {0.1, 0.75};
    double pMList[2] = {0.01, 0.1};
    double amntList[2] = {0.05, 0.25};
    
    string fileName[5] = {
        "../maxsat-problems/maxsat-crafted/bipartite/maxcut-140-630-0.8/maxcut-140-630-0.8-11.cnf",
        "../maxsat-problems/maxsat-crafted/MAXCUT/DIMACS_MOD/MANN_a27.clq.cnf",
        "../maxsat-problems/maxsat-crafted/MAXCUT/SPINGLASS/t5pm3-7777.spn.cnf",
        "../maxsat-problems/maxsat-random/highgirth/4SAT/HG-4SAT-V100-C900-23.cnf",
        "../maxsat-problems/maxsat-random/max3sat/60v/s3v60c900-4.cnf"
    };
//    string fileName[4] = {
//        "../maxsat-problems/maxsat-crafted/MAXCUT/DIMACS_MOD/MANN_a27.clq.cnf",
//        "../maxsat-problems/maxsat-crafted/MAXCUT/SPINGLASS/t5pm3-7777.spn.cnf",
//        "../maxsat-problems/maxsat-random/highgirth/4SAT/HG-4SAT-V100-C900-23.cnf",
//        "../maxsat-problems/maxsat-random/max3sat/60v/s3v60c900-4.cnf"
//    };

    int bestList[5] = {168, 408, 78, 2, 40};
    int totalBest = 0;

    
    // PBIL tests
    int count = 0;
    int numClauses = 0;
    // vary file name
    for(int f = 0; f < 5; f++) {
        cout << "Solving for " << fileName[f] << endl;
        // vary population size
        for(int i = 0; i < 3; i++) {
            // vary mutation probability
            for(int p = 0; p < 2; p++) {
                // vary mutation amount
                for(int m = 0; m < 2; m++) {
                    // vary positive learning rate
                    for(int l = 0; l < 2; l++) {
                        // vary negative learning rate
                        for(int n = 0; n < 2; n++) {
                            // vary number of generations
                            for(int g = 0; g < 3; g++) {
                                MaxSat PBILSolver(fileName[f], popSizes[i], posRate[l], negRate[p], pMList[p], amntList[m], genSizes[g]);
                                PBILSolver.solvePBIL();
//                                MaxSat GASolver(
                                cout << "Best value = " << PBILSolver.bestValue << endl;
                                cout << fileName[f] << ", " << popSizes[i] << ", " << posRate[l] << ", " << negRate[p] << ", " << pMList[p] << ", " << amntList[m] << ", " << genSizes[g] << endl;
                                if(PBILSolver.bestValue > totalBest) {
                                    totalBest = PBILSolver.bestValue;
                                }
                                numClauses = PBILSolver.numClauses;
                            }
                        }
                    }
                }
                
            }
        }
        double percentSatisfied = (double) totalBest / numClauses;
        double ratio = double (totalBest) / (numClauses - bestList[f]);
        cout << "\nBest solution satisfied " << setprecision(2) << percentSatisfied * 100 << "% (" << totalBest << "/" << numClauses << " satisfied)" << endl;
        cout << "Achieved " << setprecision(2) << ratio * 100 << "% of optimal solution" << endl;
        sleep(3);
    }
    // GA tests
}


int main (int argc, char** argv) {
    int individuals;
    string selection;
    string crossover;
    double pC;
    double pM;
    int generations;
    int algType; // 0 for GA, 1 for PBIL
    double posRate;
    double negRate;
    double mutAmnt;
    
    // holds all clauses
    vector< vector<int> > clauses;
    int numVariables;
    int numClauses;
    
    if(argc != 9) {
        printInfo();
        exit(1);
    } else {
        // type of algorithm determines how other arguments are interpreted
        if(!strcmp(argv[8], "g")) {
            cout << "doing genetic" << endl;
            algType = 0;
        } else if(!strcmp(argv[8], "p")) {
            algType = 1;
        } else {
            cout << "Invalid eighth argument specifying algorithm type. Please use:" << endl;
            cout << "    algorithm    = type of algorithm (g or p) (string)" << endl;
        }
        
        // introduces some possible errors, like entering "ts" for selection but "p" for algorithm; that error won't be caught
        if(!algType) {
            // assign relevant GA variables
            if(strcmp(argv[3], "ts") && strcmp(argv[3], "ts1") && strcmp(argv[3], "ts2") && strcmp(argv[3], "rs") && strcmp(argv[3], "bs")) {
                cout << "Invalid third argument specifying selection type. Please use:" << endl;
                cout << "   ts   = tournament selection - implies ts1" << endl;
                cout << "       ts1 = same individual cannot compete against self" << endl;
                cout << "       ts2 = same individual can compete against self" << endl;
                cout << "   rs   = rank-based selection" << endl;
                cout << "   bs   = Boltzmann selection" << endl;
                exit(1);
            } else {
                selection.assign(argv[3]);
            }
            if(strcmp(argv[4], "1c") && strcmp(argv[4], "uc")) {
                cout << "Invalid fourth argument specifying crossover type. Please use:" << endl;
                cout << "   1c   = 1-point crossover" << endl;
                cout << "   uc   = uniform crossover" << endl;
                exit(1);
            } else {
                crossover.assign(argv[4]);
            }
            pC = atof(argv[5]);
            pM = atof(argv[6]);
            
        } else {
            // assign relevant PBIL variables
            posRate = atof(argv[3]);
            negRate = atof(argv[4]);
            pM = atof(argv[5]);
            mutAmnt = atof(argv[6]);
        }
        
        individuals = atoi(argv[2]);
        generations = atoi(argv[7]);
    }
    
    
    // print correct input
    cout << "Your input values:" << endl;
    if(!algType) {
        //    cout << "    FILE_NAME = " << fileName << endl;;
        cout << "    POPULATION_SIZE =  " << individuals << endl;;
        cout << "    SELECTION       =  " << selection << endl;;
        cout << "    CROSSOVER       =  " << crossover << endl;
        cout << "    PROB_CROSS      =  " << pC << endl;
        cout << "    PROB_MUT        =  " << pM << endl;
        cout << "    MAX_GEN         =  " << generations << endl;
        cout << "    ALG_TYPE        =  GA" << endl;
    } else {
        cout << "    POPULATION_SIZE =  " << individuals << endl;;
        cout << "    POS_RATE        =  " << posRate << endl;;
        cout << "    NEG_RATE        =  " << negRate << endl;
        cout << "    PROB_MUT        =  " << pM << endl;
        cout << "    AMNT_MUT        =  " << mutAmnt << endl;
        cout << "    MAX_GEN         =  " << generations << endl;
        cout << "    ALG_TYPE        =  PBIL" << endl;
    }
    
    testCases();
    
//    if(!algType) {
//        // call GA
//        MaxSat solver(argv[1], individuals, selection, crossover, pC, pM, generations);
//        solver.solveGA();
//    } else {
//        // call PBIL
//        MaxSat solver(argv[1], individuals, posRate, negRate, pM, mutAmnt, generations);
//        solver.solvePBIL();
//    }
} // end main
