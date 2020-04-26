//
//  SimState.cpp
//  x86Simulator
//
//  Created by Rylee Mattingly on 4/25/20.
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include "SimState.hpp"

using namespace std;

// MARK: - Constructors

/**
 Construct the latency vectors for the simulator and sets setting variables.
 @param latFile     ifstream to file containing latencies.
 */
SimState::SimState(ifstream *latFile){
    
    //Instantiates the latency vectors to 1587.
    latencyList = vector<int>(1587);
    recipLatencyList = vector<int>(1587);
    
    //Set the 0th element of each list to zeros.
    latencyList[0] = 0;
    recipLatencyList[0] = 0;
    
    int indexChecker = 0;
    
    for(int i = 1; i < 1587; i++){
        *latFile >> indexChecker;
        
        //Check that the index is correct.
        if(indexChecker != i){
            cerr << "File Index Error:" << i << indexChecker;
            return;
        }
        
        //Read in the values from the file and store them.
        *latFile >> latencyList[i] >> recipLatencyList[i];
        
        //Go to next line.
        latFile->ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
}
