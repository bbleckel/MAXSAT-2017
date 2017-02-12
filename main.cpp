#include <cstdlib>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <string>
#include <fstream>

using namespace std;
// used to get target string at runtime, if desired
//private static Scanner scan = new Scanner(System.in);

void printInfo() {
    cout << endl;
    cout << "Usage:\n./main file_name individuals selection crossover pC pM generations algorithm" << endl;
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
    cout << endl;
}


int main (int argc, char** argv) {
    int individuals;
    string selection;
    string crossover;
    double pC;
    double pM;
    int generations;
    string algType;
    
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
        individuals = atoi(argv[2]);
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
        generations = atoi(argv[7]);
        if(strcmp(argv[8], "g") && strcmp(argv[8], "p")) {
            cout << "Invalid eighth argument specifying algorithm type. Please use:" << endl;
            cout << "    algorithm    = type of algorithm (g or p) (string)" << endl;
        } else {
            algType.assign(argv[8]);
        }
    
    }

    
    // print input
    cout << "Your input values:" << endl;
//    cout << "    FILE_NAME = " << fileName << endl;;
    cout << "    POPULATION_SIZE =  " << individuals << endl;;
    cout << "    SELECTION       =  " << selection << endl;;
    cout << "    CROSSOVER       =  " << crossover << endl;
    cout << "    PROB_CROSS      =  " << pC << endl;
    cout << "    PROB_MUT        =  " << pM << endl;
    cout << "    MAX_GEN         =  " << generations << endl;
    cout << "    ALG_TYPE        =  " << algType << endl;

    
} // end main
