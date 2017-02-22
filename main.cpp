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
    
    //    string fileName[11] = {"../maxsat-problems/maxsat-crafted/bipartite/maxcut-140-630-0.7/maxcut-140-630-0.7-8.cnf", "../maxsat-problems/maxsat-crafted/bipartite/maxcut-140-630-0.7/maxcut-140-630-0.7-39.cnf", "../maxsat-problems/maxsat-crafted/bipartite/maxcut-140-630-0.8/maxcut-140-630-0.8-11.cnf", "../maxsat-problems/maxsat-crafted/MAXCUT/DIMACS_MOD/brock200_4.clq.cnf", "../maxsat-problems/maxsat-crafted/MAXCUT/DIMACS_MOD/MANN_a27.clq.cnf", "../maxsat-problems/maxsat-crafted/MAXCUT/SPINGLASS/t5pm3-7777.spn.cnf", "../maxsat-problems/maxsat-random/highgirth/4SAT/HG-4SAT-V100-C900-23.cnf", "../maxsat-problems/maxsat-random/max2sat/120v/s2v120c1200-10.cnf", "../maxsat-problems/maxsat-random/max2sat/140v/s2v140c1600-6.cnf", "../maxsat-problems/maxsat-random/max3sat/60v/s3v60c900-4.cnf", "../maxsat-problems/maxsat-random/max3sat/80v/s3v80c1000-2.cnf"};
    
    // PBIL options
    int popSizes[3] = {10, 100, 500};
    int genSizes[3] = {10, 500, 2000};
    double posRate[3] = {0.01, 0.1, 0.5};
    double negRate[3] = {0.01, 0.1, 0.5};
    double pMList[3] = {0.01, 0.1, 0.5};
    double amntList[3] = {0.05, 0.25, 0.75};
    
    // GA options
    string selectList[3] = {"ts", "rs", "bs"};
    string crossoverList[2] = {"1c", "uc"};
    double crossProbList[3] = {0.01, 0.5, 0.7};
    // shares popSizes, genSizes, and pMList with PBIL
    
    
    string fileName[4] = {
        "../maxsat-problems/maxsat-crafted/bipartite/maxcut-140-630-0.8/maxcut-140-630-0.8-11.cnf",
        "../maxsat-problems/maxsat-crafted/MAXCUT/DIMACS_MOD/brock400_3.clq.cnf",
        "../maxsat-problems/maxsat-random/highgirth/4SAT/HG-4SAT-V100-C900-23.cnf",
        "../maxsat-problems/maxsat-random/max3sat/60v/s3v60c900-4.cnf"
    };
    
    double bestList[4] = {168.0, 238.0, 2.0, 40.0};
    int totalBestPBIL;
    int totalBestGA;
    
    // PBIL default arguments: 100 0.1 0.075 0.2 0.05 1000
    // GA default arguments: 100 ts 1c 0.7 0.1 1000
    
    // PBIL tests
    int count = 0;
    int numClauses = 0;
    cout << "Solving for various values... percent satisfied is relative to optimal solution, *not* overall clauses." << endl;
    sleep(4);
    
    // vary file
    for(int f = 0; f < 4; f++) {
        totalBestPBIL = 0;
        totalBestGA = 0;
        // inefficient to create new class each time, but changing that would require too many changes to be worth it at this point
        MaxSat solver(fileName[f], 100, 0.1, 0.075, 0.2, 0.05, 1000);

        cout << "Solving for " << fileName[f] << endl;
        cout << "Optimal solution is (" << solver.numClauses - bestList[f] << "/" << solver.numClauses << ") satisfied." << endl;
        
        // Solve with PBIL
        cout << "Solving with PBIL!" << endl;
        cout << "Varying population size..." << endl;
        // vary population size
        for(int i = 0; i < 3; i++) {
            MaxSat PBILSolver(fileName[f], popSizes[i], 0.1, 0.075, 0.2, 0.05, 100);
            PBILSolver.solvePBIL();
            
            numClauses = PBILSolver.numClauses;
            double ratio = PBILSolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << endl;
            } else {
                cout << "PBIL satisfied = " << setprecision(3) << ratio * 100 << "% (" << PBILSolver.bestValue << "/" << numClauses << " satisfied)" << " with " << popSizes[i] << ", " << 0.1 << ", " << 0.075 << ", " << 0.2 << ", " << 0.05 << ", " << 100 << endl;
            }
            
            if(PBILSolver.bestValue > totalBestPBIL) {
                totalBestPBIL = PBILSolver.bestValue;
            }
        }
        
        cout << "Varying mutation probability..." << endl;
        // vary mutation probability
        for(int p = 0; p < 3; p++) {
            MaxSat PBILSolver(fileName[f], 100, 0.1, 0.075, pMList[p], 0.05, 100);
            PBILSolver.solvePBIL();
            
            double ratio = PBILSolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << endl;
            } else {
                cout << "PBIL satisfied = " << setprecision(3) << ratio * 100 << "% (" << PBILSolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", " << 0.1 << ", " << 0.075 << ", " << pMList[p] << ", " << 0.05 << ", " << 100 << endl;
            }
            
            if(PBILSolver.bestValue > totalBestPBIL) {
                totalBestPBIL = PBILSolver.bestValue;
            }
        }
        
        cout << "Varying mutation amount..." << endl;
        // vary mutation amount
        for(int m = 0; m < 3; m++) {
            MaxSat PBILSolver(fileName[f], 100, 0.1, 0.075, 0.2, amntList[m], 100);
            PBILSolver.solvePBIL();
            
            double ratio = PBILSolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << endl;
            } else {
                cout << "PBIL satisfied = " << setprecision(3) << ratio * 100 << "% (" << PBILSolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", " << 0.1 << ", " << 0.075 << ", " << 0.2 << ", " << amntList[m] << ", " << 100 << endl;
            }
            
            if(PBILSolver.bestValue > totalBestPBIL) {
                totalBestPBIL = PBILSolver.bestValue;
            }
        }
        
        cout << "Varying positive learning rate..." << endl;
        // vary positive learning rate
        for(int l = 0; l < 3; l++) {
            MaxSat PBILSolver(fileName[f], 100, posRate[l], 0.075, 0.2, 0.05, 100);
            PBILSolver.solvePBIL();
            
            double ratio = PBILSolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << endl;
            } else {
                cout << "PBIL satisfied = " << setprecision(3) << ratio * 100 << "% (" << PBILSolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", " << posRate[l] << ", " << 0.075 << ", " << 0.2 << ", " << 0.05 << ", " << 100 << endl;
            }
            
            if(PBILSolver.bestValue > totalBestPBIL) {
                totalBestPBIL = PBILSolver.bestValue;
            }
        }
        
        cout << "Varying negative learning rate..." << endl;
        // vary negative learning rate
        for(int n = 0; n < 3; n++) {
            MaxSat PBILSolver(fileName[f], 100, 0.1, negRate[n], 0.2, 0.05, 100);
            PBILSolver.solvePBIL();
            
            double ratio = PBILSolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << endl;
            } else {
                cout << "PBIL satisfied = " << setprecision(3) << ratio * 100 << "% (" << PBILSolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", " << 0.1 << ", " << negRate[n] << ", " << 0.2 << ", " << 0.05 << ", " << 100 << endl;
            }
            
            if(PBILSolver.bestValue > totalBestPBIL) {
                totalBestPBIL = PBILSolver.bestValue;
            }
        }
        
        cout << "Varying generation number..." << endl;
        // vary number of generations
        for(int g = 0; g < 3; g++) {
            MaxSat PBILSolver(fileName[f], 100, 0.1, 0.075, 0.2, 0.05, genSizes[g]);
            PBILSolver.solvePBIL();
            
            double ratio = PBILSolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << endl;
            } else {
                cout << "PBIL satisfied = " << setprecision(3) << ratio * 100 << "% (" << PBILSolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", " << 0.1 << ", " << 0.075 << ", " << 0.2 << ", " << 0.05 << ", " << genSizes[g] << endl;
            }
            
            if(PBILSolver.bestValue > totalBestPBIL) {
                totalBestPBIL = PBILSolver.bestValue;
            }
        }
        
        // Solve with GA
        cout << "Solving with GA!" << endl;
        
        cout << "Varying population size..." << endl;
        // vary population size
        for(int i = 0; i < 3; i++) {
            MaxSat GASolver(fileName[f], popSizes[i], "ts", "1c", 0.7, 0.1, 100);
            GASolver.solveGA();
            numClauses = GASolver.numClauses;
            double ratio = GASolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << " (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << popSizes[i] << ", ts, 1c, " << 0.7 << ", " << 0.1 << ", " << 100 << endl;
            } else {
                cout << "GA satisfied = " << setprecision(3) << ratio * 100 << "% (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << popSizes[i] << ", ts, 1c, " << 0.7 << ", " << 0.1 << ", " << 100 << endl;
            }
            if(GASolver.bestValue > totalBestGA) {
                totalBestGA = GASolver.bestValue;
            }
        }
        
        cout << "Varying mutation probability..." << endl;
        // vary mutation probability
        for(int p = 0; p < 3; p++) {
            MaxSat GASolver(fileName[f], 100, "ts", "1c", 0.7, pMList[p], 100);
            GASolver.solveGA();
            
            double ratio = GASolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << " (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", ts, 1c, " << 0.7 << ", " << pMList[p] << ", " << 100 << endl;
            } else {
                cout << "GA satisfied = " << setprecision(3) << ratio * 100 << "% (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", ts, 1c, " << 0.7 << ", " << pMList[p] << ", " << 100 << endl;
            }
            
            if(GASolver.bestValue > totalBestGA) {
                totalBestGA = GASolver.bestValue;
            }
        }
        
        cout << "Varying selection type..." << endl;
        // vary selection type
        for(int m = 0; m < 3; m++) {
            MaxSat GASolver(fileName[f], 100, selectList[m], "1c", 0.7, 0.1, 100);
            GASolver.solveGA();
            
            double ratio = GASolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << " (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", " << selectList[m] << " 1c, " << 0.7 << ", " << 0.1 << ", " << 100 << endl;
            } else {
                cout << "GA satisfied = " << setprecision(3) << ratio * 100 << "% (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << 100 << ", " << selectList[m] << " 1c, " << 0.7 << ", " << 0.1 << ", " << 100 << endl;
            }
            
            if(GASolver.bestValue > totalBestGA) {
                totalBestGA = GASolver.bestValue;
            }
        }
        
        cout << "Varying crossover type..." << endl;
        // vary crossover type
        for(int l = 0; l < 2; l++) {
            MaxSat GASolver(fileName[f], 100, "ts", crossoverList[l], 0.7, 0.1, 100);
            GASolver.solveGA();
            
            double ratio = GASolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << " (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", ts, " << crossoverList[l] << ", " << 0.7 << ", " << 0.1 << ", " << 100 << endl;
            } else {
                cout << "GA satisfied = " << setprecision(3) << ratio * 100 << "% (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", ts, " << crossoverList[l] << ", " << 0.7 << ", " << 0.1 << ", " << 100 << endl;
            }
            
            if(GASolver.bestValue > totalBestGA) {
                totalBestGA = GASolver.bestValue;
            }
        }
        
        cout << "Varying crossover probability..." << endl;
        // vary crossover probability
        for(int n = 0; n < 3; n++) {
            MaxSat GASolver(fileName[f], 100, "ts", "1c", crossProbList[n], 0.1, 100);
            GASolver.solveGA();
            
            double ratio = GASolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << " (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", ts, 1c, " << crossProbList[n] << ", " << 0.1 << ", " << 100 << endl;
            } else {
                cout << "GA satisfied = " << setprecision(3) << ratio * 100 << "% (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", ts, 1c, " << crossProbList[n] << ", " << 0.1 << ", " << 100 << endl;
            }
            
            if(GASolver.bestValue > totalBestGA) {
                totalBestGA = GASolver.bestValue;
            }
        }
        
        cout << "Varying generation number..." << endl;
        // vary number of generations
        for(int g = 0; g < 3; g++) {
            MaxSat GASolver(fileName[f], 100, "ts", "1c", 0.7, 0.1, genSizes[g]);
            GASolver.solveGA();
            
            double ratio = GASolver.bestValue / (numClauses - bestList[f]);
            if(ratio > 1) {
                cout << "Beat optimal solution!" << " (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", ts, 1c, " << 0.7 << ", " << 0.1 << ", " << genSizes[g] << endl;
            } else {
                cout << "GA satisfied = " << setprecision(3) << ratio * 100 << "% (" << GASolver.bestValue << "/" << numClauses << " satisfied)" << " with " << 100 << ", ts, 1c, " << 0.7 << ", " << 0.1 << ", " << genSizes[g] << endl;
            }
            
            if(GASolver.bestValue > totalBestGA) {
                totalBestGA = GASolver.bestValue;
            }
        }
        
        double percentSatisfiedPBIL = (double) totalBestPBIL / numClauses;
        double ratioPBIL = double (totalBestPBIL) / (numClauses - bestList[f]);
        double percentSatisfiedGA = (double) totalBestGA / numClauses;
        double ratioGA = double (totalBestGA) / (numClauses - bestList[f]);
        
        // give overall results of file
        cout << "\nPBIL best solution satisfied " << setprecision(3) << ratioPBIL * 100 << "% (" << totalBestPBIL << "/" << numClauses << " satisfied)" << endl;

        cout << "GA best solution satisfied " << setprecision(3) << ratioGA * 100 << "% (" << totalBestGA << "/" << numClauses << " satisfied)" << endl;
        sleep(3);
    }
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

//        if(!algType) {
//            // call GA
//            MaxSat solver(argv[1], individuals, selection, crossover, pC, pM, generations);
//            solver.solveGA();
//        } else {
//            // call PBIL
//            MaxSat solver(argv[1], individuals, posRate, negRate, pM, mutAmnt, generations);
//            solver.solvePBIL();
//
//            cout << "PBIL (" << solver.bestValue << "/" << solver.numClauses << " satisfied)" << endl;
//        }
} // end main
