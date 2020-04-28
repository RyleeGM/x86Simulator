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
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "FileOperations.hpp"

class Instruction{
    
private:
    //Instruction Characteristics
    bool execute;
    uint64_t instr_Ptr;
    int size;
    int opcode;
    //Repeat Characteristics
    bool rep;
    int num_Rep;    //Optional
    //Register Read
    int num_RegR;
    std::vector<int> reg_R_Name; //Optional
    std::vector<int> reg_R_Size; //Optional
    //Memory Read
    int num_MemR;
    std::vector<uint64_t> mem_R_Addr;   //Optional
    std::vector<int> mem_R_Size;        //Optional
    //Memory Write
    int num_MemW;
    std::vector<uint64_t> mem_W_Addr;   //Optional
    std::vector<int> mem_W_Size;        //Optional
    //Register Write
    int num_RegW;
    std::vector<int> reg_W_Name;        //Optional
    std::vector<int> reg_W_Size;        //Optional
    
    //Simulation State Variables
    bool sidelined;
    int recall;
    int latency;
    float recipLatency;
    
public:
    //Constructor
    Instruction();
    Instruction(std::ifstream *insFile);
    
    //Accessor Methods
    int largestRegR();
    int largestMemR();
    int largestMemW();
    int largestRegW();
    bool isProducer(std::vector<int> regs);
    std::vector<uint64_t> getReadAddr();
    std::vector<uint64_t> getWriteAddr();
    std::vector<int> getRegsRead();
    int getRep();
    float getRecipLatency();
    int getOpcode();
    int getSize();
    
    //State Accessors
    int getLoadBlocks();
    int getStoreBlocks();
    bool isSidelined();
    bool readyForRecall();
    bool readyForCommit();
    
    //State Setters
    void setSidelineStatus(bool status);
    void setRecallTime(int time);
    void setLatency(int lat);
    void setRecipLatency(float recip);
    
    //Overloaded Operators
    friend std::ostream& operator<< (std::ostream& s, Instruction& ins);
    void  operator-- ();
    
};





#endif /* Instruction_hpp */
