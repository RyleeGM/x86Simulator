//
//  SimState.cpp
//  x86Simulator
//
//  Created by Rylee Mattingly on 4/25/20.
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include "SimState.hpp"

using namespace std;

// MARK: - Managing Functions

/**
 This function manages the issue of instruction.
 */
void SimState::issue(){
    
    //Keep track of the instructions commited this cycle
    //-Needed to not break reciprical latency requirements-
    list<tuple<int,int>> instructionsThisCycle;
    
    //Check if the first instruction in the buffer FIFO has a producer
    //in progress.
    issuedIns;
    
}

/**
    This function commits the functions that have completed their work.
 */
void SimState::commit(){
    
    for(list<Instruction>::iterator it = issuedIns.begin(); it != issuedIns.end(); it++){
        //Check if instruction is ready for commit.
        if(it->readyForCommit()){
            //Release functional blocks
            availableLoads += it->getLoadBlocks();
            availableStores += it->getStoreBlocks();
            availableALU += 1;
            
            //Remove from list.
            issuedIns.erase(it);
        }
    }
    return;
}

// MARK: - Constructors

/**
 Construct the latency vectors for the simulator and sets setting variables.
 @param latFile     ifstream to file containing latencies. No header.
 @param setFile     ifstream to file containing settings. No header.
 */
SimState::SimState(ifstream *latFile, ifstream *setFile){
    
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
    
    //Read in settings.
    changeSettings(setFile);
    
    //Set available resource blocks.
    availableLoads = loadBlocks;
    availableStores = storeBlocks;
    availableALU = aluBlocks;
    
    return;
}

// MARK: - Mutators

/**
 Change the settings of the simulator. This will eventually call  Reset()
 @param setFile     ifstream to file containing settings. No header.
 */
void SimState::changeSettings(std::ifstream *setFile){
    
    //Read in settings and clear each comment line.
    *setFile >> fetchWidth;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> issueWidth;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> loadBlocks;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> storeBlocks;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> aluBlocks;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> isBlockingCache;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    return;
}

/**
 This function will start and run the entire simulation until there are no more instructions in the trace.
 */
