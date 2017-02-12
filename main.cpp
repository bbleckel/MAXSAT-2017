#include <cstdlib>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <string>
#include <fstream>
#include "PBIL.h"

using namespace std;
// used to get target string at runtime, if desired
//private static Scanner scan = new Scanner(System.in);

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
    cout << "                     2c   = 2-point crossover" << endl;
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
    
    if(argc != 9) {
        printInfo();
        exit(1);
    } else {
        // deal with reading file
        //        ifstream fileName(argv[1]);
        //        if(!fileName.is_open()) {
        //            cout << "ERROR: Could not open file" << endl;
        //        } else {
        //            char x;
        //            while(fileName.get(x)) {
        //                // potentially useful file reading implementation from cachesim 2330
        //                cout << x;
        //            }
        //        }
        // type of algorithm determines how other arguments are interpreted
        if(!strcmp(argv[8], "g")) {
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
            if(strcmp(argv[4], "1c") && strcmp(argv[4], "2c") && strcmp(argv[4], "uc")) {
                cout << "Invalid fourth argument specifying crossover type. Please use:" << endl;
                cout << "   1c   = 1-point crossover" << endl;
                cout << "   2c   = 2-point crossover" << endl;
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
        cout << "    ALG_TYPE        =  " << algType << endl;
    } else {
        cout << "    POPULATION_SIZE =  " << individuals << endl;;
        cout << "    POS_RATE        =  " << posRate << endl;;
        cout << "    NEG_RATE        =  " << negRate << endl;
        cout << "    PROB_MUT        =  " << pM << endl;
        cout << "    AMNT_MUT        =  " << mutAmnt << endl;
        cout << "    MAX_GEN         =  " << generations << endl;
        cout << "    ALG_TYPE        =  " << algType << endl;
    }
    
    if(!algType) {
        // call GA
    } else {
        // call PBIL
        PBIL_MAXSAT(individuals, posRate, negRate, pM, mutAmnt, generations, 10);
    }
    
    
} // end main
