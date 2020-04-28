//
//  Instruction.cpp
//  x86Simulator
//
//  Created by Rylee Mattingly on 4/1/20.
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include "Instruction.hpp"

using namespace std;

// MARK: - Constructors

/**
Default constructor that initializes the vectors and optional sizes to 0.
 */
Instruction::Instruction(){
    num_RegR = 0;
    reg_R_Name = vector<int>();
    reg_R_Size = vector<int>();
    num_MemR = 0;
    mem_R_Addr = vector<uint64_t>();
    mem_R_Size = vector<int>();
    num_MemW = 0;
    mem_W_Addr = vector<uint64_t>();
    mem_W_Size = vector<int>();
    num_RegW = 0;
    reg_W_Name = vector<int>();
    reg_W_Size = vector<int>();
}

/**
 Creates an instruction object
 Returns with the file set up for the next instruction.
 @param insFile     ifstream that can be used to get data from the trace. Header must be pre-removed.
 */
Instruction::Instruction(ifstream *insFile){
    
    //Read in the execution bit, instruction pointer, size and opcode.
    execute = getNextbool(insFile);
    *insFile >> hex >> instr_Ptr;
    size = getNextInt(insFile);
    opcode = getNextInt(insFile);
    
    //Read in repeat.
    rep = getNextbool(insFile);
    
    if(rep){
        num_Rep = getNextInt(insFile);
    }
    
    //Read in Read Registers and instantiate arrays.
    num_RegR = getNextInt(insFile);
    reg_R_Name = vector<int>(num_RegR);
    reg_R_Size = vector<int>(num_RegR);
    
    for(int i = 0; i < num_RegR; i++){
        reg_R_Name[i] = getNextInt(insFile);
        reg_R_Size[i] = getNextInt(insFile);
    }
    
    //Read in Memory Read and instantiate arrays.
    *insFile >> num_MemR;
    mem_R_Addr = vector<uint64_t>(num_MemR);
    mem_R_Size = vector<int>(num_MemR);
    
    for(int i = 0; i < num_MemR; i++){
        *insFile >> mem_R_Addr[i];
        mem_R_Size[i] = getNextInt(insFile);
    }
    
    //Read in Memory Written and instantiate arrays.
    *insFile >> num_MemW;
    mem_W_Addr = vector<uint64_t>(num_MemW);
    mem_W_Size = vector<int>(num_MemW);
    
    for(int i = 0; i < num_MemW; i++){
        *insFile >> mem_W_Addr[i];
        mem_W_Size[i] = getNextInt(insFile);
    }
    
    //Read in Written Registers and instantiate arrays.
    *insFile >> num_RegW;
    reg_W_Name = vector<int>(num_RegW);
    reg_W_Size = vector<int>(num_RegW);
    
    for(int i = 0; i < num_RegW; i++){
        reg_W_Name[i] = getNextInt(insFile);
        reg_W_Size[i] = getNextInt(insFile);
    }
    
    //Values set at instruction fetch time.
    sidelined = false;
    recall = 0;
    latency = 0;
    recipLatency = 0;
    
    return;
}

// MARK: - Accessors
/**
 Returns the size of the largest register read in bytes.
 */
int Instruction::largestRegR(){
    
    //Hold the largest size.
    int largestSize = 0;
    
    //Iterate over the sizes keeping only the largest.
    for(int i = 0; i < num_RegR; i++){
        if(reg_R_Size[i] > largestSize){
            largestSize = reg_R_Size[i];
        }
    }
    
    //Return the largest size that was seen.
    return largestSize;
}

/**
 Returns the size of the largest mem operand read in bytes.
 */
int Instruction::largestMemR(){
    
    //Hold the largest size.
    int largestSize = 0;
    
    //Iterate over the sizes keeping only the largest.
    for(int i = 0; i < num_MemR; i++){
        if(mem_R_Size[i] > largestSize){
            largestSize = mem_R_Size[i];
        }
    }

    //Return the largest size.
    return largestSize;
}

/**
 Returns the size of the largest mem operand written in bytes.
 */
int Instruction::largestMemW(){
    
    //Hold the largest size.
    int largestSize = 0;
    
    //Iterate over the sizes and keep only the largest.
    for(int i = 0; i < num_MemW; i++){
        if(mem_W_Size[i] > largestSize){
            largestSize = mem_W_Size[i];
        }
    }
    
    //Return the largest size that was seen.
    return largestSize;
}

/**
 Returns the size of the largest register written in bytes.
 @return largest    Largest register written in bytes.
 */
int Instruction::largestRegW(){
    
    //Hold the largest size.
    int largestSize = 0;
    
    //Iterate over the sizes and keep only the largest.
    for(int i = 0; i < num_RegW; i++){
        if(reg_W_Size[i] > largestSize){
            largestSize = reg_W_Size[i];
        }
    }
    //Return the largest size that was seen.
    return largestSize;
}

/**
 @param regs      A list of register names to compare against.
 @return producer   A bool describing if this instruction writes the passed registers.
 */
bool Instruction::isProducer(vector<int> regs){
    //Iterate over registers written by this instruction.
    for(int i = 0; i < num_RegW; i++){
        //Compare each written reg to each register from consumer.
        for(int j = 0; j < regs.size(); i++){
            //If the registers are equal return true.
            if(regs[j] == reg_W_Name[i]){
                return true;
            }
        }
    }
    return false;
}

/**
 @return memRAddrs   A list of memory addresses read by the instruction.
 */
vector<uint64_t> Instruction::getReadAddr(){
    
    //Make a copy to protect data.
    vector<uint64_t> temp = mem_R_Addr;
    
    return temp;
}

/**
 @return memWAddrs  A list of memory addresses written by the instruction.
 */
vector<uint64_t> Instruction::getWriteAddr(){
    
    //Make a copy to protect data.
    vector<uint64_t> temp = mem_W_Addr;
    
    return temp;
}

/**
 @return regNames   An array of the names of registers.
 */
vector<int> Instruction::getRegsRead(){
    
    //Make a copy to protect data.
    vector<int> temp = reg_R_Name;
    
    return temp;
}

/**
 @return repeat     The number of times this instruction repeats
 */
int Instruction::getRep(){
    if(rep)
        return num_Rep;
    else
        return 0;
}

/**
 @return    return the value of the reciprical latency.
 */
float Instruction::getRecipLatency(){
    return recipLatency;
}

/**
 @return    return integer value for the opcode.
 */
int Instruction::getOpcode(){
    return opcode;
}

/**
 @return    returns the size of the instruction in bytes.
 */
int Instruction::getSize(){
    
    return size;
}

// MARK: - State Accessors

/**
 @return loadBlocks     Returns load blocks needed for this instruction.
 */
int Instruction::getLoadBlocks(){
    return num_MemR;
}

/**
 @Return storeBlocks    Returns store blocks needed for this instruction.
 */
int Instruction::getStoreBlocks(){
    return num_MemW;
}

/**
 @return isSidelined    Returns a bool to describe if instruction is waiting for mem load.
 */
bool Instruction::isSidelined(){
    return sidelined;
}

/**
 @return ready      Returns a bool to describe if an instruction is ready to be executed after being sidelined.
 */
bool Instruction::readyForRecall(){
    //If it isn't sidelined and there isn't time on the recall clock.
    if(sidelined == false && recall == 0)
        return true;
    else
        return false;
}

/**
 @return    ready  Returns a bool to describe if an instruction is ready to commit.
 */
bool Instruction::readyForCommit(){
    //If there isn't time on the scoreboard.
    if(latency == 0)
        return true;
    else
        return false;
}

// MARK: - State Setters

/**
 Note: A recall time should be set if an instruction is marked as sidelined.
 @param status      A bool to describe the sideline status of the instruction.
 */
void Instruction::setSidelineStatus(bool status){
    sidelined = status;
}

/**
 Note: The sideline flag should be set when a recall value is set.
 @param time    an integer representation of the number of cyctles the instruction is supposed to be sidelined.
 */
void Instruction::setRecallTime(int time){
    recall = time;
}

/**
 @param lat     an integer to describe the amount of time the instruction needs to execute.
 */
void Instruction::setLatency(int lat){
    latency = lat;
}

/**
 @param recip   an integer to describe how many cycles before another independent instruction of the same type can be executed.
 */
void Instruction::setRecipLatency(float recip){
    recipLatency = recip;
}

// MARK: - Operators

/**
 Print out the instruction as you would find it in the trace file.
 @param s       Referance to ostream operator
 @param ins     Reference to instruction
 */
ostream& operator<< (ostream& s, Instruction& ins){
    
    //Print out the execution bit, instr_ptr, size and opcode.
    s << ins.execute << " " <<  hex << ins.instr_Ptr << " ";
    s << ins.size << " " << ins.opcode << " ";
    
    //Print repeat flag.
    s << ins.rep << " ";
    
    //Print num of read registers and read memory.
    s << ins.num_RegR << " " << ins.num_MemR << " ";
    //Pring num of written memory and written registers.
    s << ins.num_MemW << " "  << ins.num_RegW << " ";
    
    return s;
}

/**
 If the instruction is sidelined then decrement recall otherwise decrement latency.
 Sets sidelined to false if the recall time is set to zero.
 */
void Instruction::operator-- (){
    
    //Check to see if the instruction is sidelined;
    if(sidelined){
        //Decrement the recall time.
        recall = recall - 1;
        //Check if still needs to be sidelined.
        if(recall == 0){
            sidelined = false;
        }
    } else {
        //Decrement the latency and reciprical latency.
        latency = latency - 1;
        recipLatency = recipLatency - 1;
    }
}
