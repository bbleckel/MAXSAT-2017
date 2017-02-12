#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

using namespace std;



void PBIL_MAXSAT(int individuals, double posRate, double negRate, double pM, double mutAmnt, int generations, int stringSize) {
    cout << "Solving with PBIL..." << endl;
    
    vector<double> PV;
    
    // initialize PV
    for(int i = 0; i < stringSize; i++) {
        PV.push_back(0.5);
    }
}
