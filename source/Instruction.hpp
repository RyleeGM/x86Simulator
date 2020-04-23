//
//  Instruction.hpp
//  x86Simulator
//
//  Created by Rylee Mattingly on 4/1/20.
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#ifndef Instruction_hpp
#define Instruction_hpp

#include <stdio.h>
#include <stdint.h>

class Instruciton{
    
private:
    //Instruction Characteristics
    bool execute;
    uint64_t address;
    int length;
    int opcode;
    //Regitsters Read
    int numRegsRead;
    int* regsReadArray;
    //Memory Read
    int numMemRead;
    uint64_t* memReadArray;
    //Memory Written
    int numMemWritten;
    uint64_t* memWrittenArray;
    //Registers Written
    int numRegsWritten;
    int* regsWrittenArray;
    //Execution Characteristics
    int latency;
    
public:
    //Constructors
    
    
};





#endif /* Instruction_hpp */
