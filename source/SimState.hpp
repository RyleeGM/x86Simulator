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
#include <queue>
#include <tuple>
#include "Instruction.hpp"

class SimState{
  
private:
    //Master Lists for latency and reciprical latency.
    std::vector<int> latencyList;
    std::vector<float> recipLatencyList;
    
    //List of instructions in the repective sections of the simulator.
    std::queue<Instruction> fetchedIns;
    std::list<Instruction> issuedIns;
    
    //Structure to manage cycles until next instruction of a given type.
    std::list<std::tuple<int, int>> recipCount;
    
    //User set variables from settings file.
    int fetchWidth;
    int issueWidth;
    int loadBlocks;
    int storeBlocks;
    int aluBlocks;
    bool isBlockingCache;
    int fetchBufferSize;
    //Variables to track resource availability.
    int availableStores;
    int availableLoads;
    int availableALU;
    //Reporting Variables;
    uint64_t cycleCount;
    
    //Managing Function
    void fetch(std::ifstream *trace);
    void issue();
    void commit();
    void sideline();
    
    //Helper Functions
    int getLatency(Instruction *ins);
    
public:
    //Constructors
    SimState(std::ifstream *latFile, std::ifstream *setFile);
    
    //Mutators
    void changeSettings(std::ifstream *setFile);

    //Initiation and Termination
    void manager(std::ifstream *trace);
    void cycle(std::ifstream *trace);
    void reset();
    void report();
    
    //CommitHelper
    bool checkIssueEligibility();
    
    //Latency Helper
    void latencyHelper(Instruction *ins);
};

#endif /* SimState_hpp */
