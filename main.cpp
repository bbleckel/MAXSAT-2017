#include <cstdlib>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <string>

using namespace std;
// used to get target string at runtime, if desired
//private static Scanner scan = new Scanner(System.in);


int main (int argc, char** argv) {
    // default values
    int individuals = 100;
    string selection;
    selection.assign("ts");
    int elitism = 0;
    string crossover;
    crossover.assign("1c");
    double pC = 0.7;
    double pM = 0.7;
    int generations = 1000;
    int disInterval = 10;
    
    int c;
    
    /* Parse the command line */
    /* b: BASIC, g: GREEDY, r: RANDOM, s: SMART-START, f: FIRST-BETTER, n: BOARD_SIZE, N: MAX_ATTEMPTS, h: HELP, d: Display Boards */
    while ((c = getopt(argc, argv, "i:s:ec:r:m:g:d:h")) != EOF) {
        switch (c) {
            case 'i':
                individuals = atoi(optarg);
                break;
            case 's':
                selection.assign(optarg);
                break;
            case 'e':
                elitism = 1;
                break;
            case 'c':
                crossover.assign(optarg);
                break;
            case 'r':
                pC = atof(optarg);
                break;
            case 'm':
                pM = atof(optarg);
                break;
            case 'g':
                generations = atoi(optarg);
                break;
            case 'd':
                disInterval = atoi(optarg);
                break;
            case 'h':
                cout << "Arguments [i:s:ec:r:m:g:d:h] (each has a default - see README.txt)" << endl;
                return 0;

        }
    }
    
    // print input
    cout << "Your input values (if you did not change one, it will be its default value):" << endl;
    cout << "    POPULATION_SIZE = " << individuals << endl;;
    cout << "    SELECTION = " << selection << endl;;
    cout << "    ELITE = " << elitism << endl;
    cout << "    CROSSOVER = " << crossover << endl;
    cout << "    PROB_CROSS = " << pC << endl;
    cout << "    PROB_MUTE = " << pM << endl;
    cout << "    MAX_GEN = " << generations << endl;
    cout << "    DISP_INTERVAL = " << disInterval << endl;
    
    // process command line arguments
    //    if (argc != 8) {
//        cout << endl;
//        cout << "Usage:\n./main individuals selection elitism crossover pC pM generations disInterval" << endl;
//        cout <<
//        cout << "    individuals  = number of individuals in population (int)" << endl;
//        cout << "    selection    = type of selection of breeding pool (string):" << endl;
//        cout << "                     ts   = tournament selection - implies ts1" << endl;
//        cout << "                            ts1 = same individual cannot compete against self" << endl;
//        cout << "                            ts2 = same individual can compete against self" << endl;
//        cout << "                     rs   = rank based selection" << endl;
//        cout << "                     bs   = Boltzmann selection" << endl;
//        cout << "    elitism      = use of elitism (string):" << endl;
//        cout << "                     et   = use elitism" << endl;
//        cout << "                     ef   = do not use elitism" << endl;
//        cout << "    crossover    = crossover method (string):" << endl;
//        cout << "                     1c   = 1-point crossover" << endl;
//        cout << "                     2c   = 2-point crossover" << endl;
//        cout << "                     uc   = uniform crossover" << endl;
//        cout << "    pC           = crossover probability (double)" << endl;
//        cout << "    pM           = mutation probability (double)" << endl;
//        cout << "    generations  = max number of generations to run (int)" << endl;
//        cout << "    disInterval  = show best interval (int)" << endl;
//        cout <<  << endl;
//        return 0; // end if inputs incorrect
//    } else {
//        // convert command line strings to values
//        GA.POPULATION_SIZE = Integer.parseInt(args[0]);
//        if (args[1].equals(GA.TS) || args[1].equals(GA.TS1) || args[1].equals(GA.TS2) |
//            args[1].equals(GA.RS) || args[1].equals(GA.BS)) {
//            GA.SELECTION = args[1];
//        }
//        else {
//            cout << "Invalid second argument specifying selection type.  Please use:");
//            cout << "    ts   = tournament selection - implies ts1");
//            cout << "          ts1 = same individual cannot compete against self");
//            cout << "          ts2 = same individual can compete against self");
//            cout << "    rs   = rank based selection");
//            cout << "    bs   = Boltzmann selection");
//            System.exit(1);
//        }
//        if (args[2].equals(GA.E_ON)) {
//            GA.ELITE = true;
//        }
//        else if (args[2].equals(GA.E_OFF)) {
//            GA.ELITE = false;
//        }
//        else {
//            cout << "Invalid third argument specifying elitism.  Please use:");
//            cout << "    et   = use elitism");
//            cout << "    ef   = do not use elitism");
//            System.exit(1);
//        }
//        if (args[3].equals(GA.ONE_C) || args[3].equals(GA.TWO_C) || args[3].equals(GA.UNIFORM) || args[3].equals("buc") || args[3].equals("suc")) {
//            GA.CROSSOVER = args[3];
//        }
//        else {
//            cout << "Invalid fourth argument specifying crossover type.  Please use:");
//            cout << "    1c   = 1-point crossover");
//            cout << "    2c   = 2-point crossover");
//            cout << "    uc   = uniform crossover");
//            System.exit(1);
//        }
//        GA.PROB_CROSS = Double.parseDouble(args[4]);
//        GA.PROB_MUTE = Double.parseDouble(args[5]);
//        
//        GA.MAX_GEN = Integer.parseInt(args[6]);
//        GA.DISP_INTERVAL = Integer.parseInt(args[7]);
//        
//        // print out the values supplied
//        // 	     cout << );
//        // 	     cout << "The following are the values you entered:");
//        // 	     cout << "    POPULATION_SIZE = " + GA.POPULATION_SIZE);
//        // 	     cout << "    SELECTION = " + GA.SELECTION);
//        // 	     cout << "    ELITE = " + GA.ELITE);
//        // 	     cout << "    CROSSOVER = " + GA.CROSSOVER);
//        // 	     cout << "    PROB_CROSS = " + GA.PROB_CROSS);
//        // 	     cout << "    PROB_MUTE = " + GA.PROB_MUTE);
//        // 	     cout << "    MAX_GEN = " + GA.MAX_GEN);
//        // 	     cout << "    DISP_INTERVAL = " + GA.DISP_INTERVAL);
//    }
//    
//    // you can modify the alphabet from which it is assumed the target string is drawn
//    GA.LOWER_CASE = "abcdefghijklmnopqrstuvwxyz";
//    GA.UPPER_CASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//    GA.DIGITS = "0123456789";
//    GA.SYMBOLS = "!@#$%^&*()_+=-[]\\{}|;':\",./<>?";
//    GA.SPACE = " ";
//    GA.ALPHABET = GA.LOWER_CASE + GA.UPPER_CASE + GA.DIGITS + GA.SYMBOLS + GA.SPACE;
//    
//    
//    // use code block 1 and comment out code block 2 to hardcode the target string
//    // comment out code block 1 and use code block 2 to inpout the string at run time
//    
//    // code block 1
//    //	String targetString = "I think this is a reasonable medium sized string!!";
//    
//    // code block 2
//    System.out.print("Enter the string to be generated: ");
//    String targetString = scan.nextLine();
//    
//    // call the method to run the GA
//    GA.runGA(targetString);
    
} // end main
