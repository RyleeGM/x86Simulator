//
//  SimState.hpp
//  x86Simulator
//
//  Created by Rylee Mattingly on 4/25/20.
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#ifndef SimState_hpp
#define SimState_hpp

#include <list>
#include "Instruction.hpp"

class SimState{
  
private:
    //Master Lists for latency and reciprical latency.
    std::vector<int> latencyList;
    std::vector<int> recipLatencyList;
    //List of instructions in the repective sections of the simulator.
    std::list<Instruction> fetchedIns;
    std::list<Instruction> issuedIns;
    //User set variables from settings file.
    int issueWidth;
    int fetchWidth;
    int loadBlocks;
    int storeBlocks;
    int aluBlocks;
    //Variables to track resource availability.
    int availableStores;
    int availableLoads;
    int availableALU;
    
public:
    //Constructors
    SimState(std::ifstream *latFile);
    
    
    
};



#endif /* SimState_hpp */
