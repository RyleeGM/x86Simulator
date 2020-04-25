//
//  Instruction.cpp
//  x86Simulator
//
//  Created by Rylee Mattingly on 4/1/20.
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include "Instruction.hpp"

using namespace std;

// Mark: - Constructors

/**
 Creates an instruction object
 This Function returns with the file after the new line.
 @param insFile     ifstream that can be used to get data from the trace.
 */
Instruction::Instruction(ifstream *insFile){
    
    //Read in the execution bit, instruction pointer, size and opcode.
    *insFile >> execute >> hex >> instr_Ptr >> size >> opcode;
    
    //Read in repeat.
    *insFile >> rep;
    
    if(rep){
        *insFile >> num_Rep;
    }
    
    //Read in Read Registers and instantiate arrays.
    *insFile >> num_RegR;
    reg_R_Name = new int [num_RegR];
    reg_R_Size = new int [num_RegR];
    
    for(int i = 0; i < num_RegR; i++){
        *insFile >> *(reg_R_Name + i) >> *(reg_R_Size + i);
    }
    
    //Read in Memory Read and instantiate arrays.
    *insFile >> num_MemR;
    mem_R_Addr = new uint64_t[num_MemR];
    mem_R_Size = new int[num_MemR];
    
    for(int i = 0; i < num_MemR; i++){
        *insFile >> hex >> *(mem_R_Addr + i) >> *(mem_R_Size + i);
    }
    
    //Read in Memory Written and instantiate arrays.
    *insFile >> num_MemW;
    mem_W_Addr = new uint64_t[num_MemW];
    mem_W_Size = new int[num_MemW];
    
    for(int i = 0; i < num_MemW; i++){
        *insFile >> hex >> *(mem_W_Addr + i) >> *(mem_W_Size + i);
    }
    
    //Read in Written Registers and instantiate arrays.
    *insFile >> num_RegW;
    reg_W_Name = new int [num_RegW];
    reg_W_Size = new int [num_RegW];
    
    for(int i = 0; i < num_RegW; i++){
        *insFile >> *(reg_W_Name + i) >> *(reg_W_Size + i);
    }
    
    return;
}

/**
 Delete the Instrction
 */
Instruction::~Instruction(){
    delete reg_R_Size;
    delete reg_R_Name;
    delete mem_W_Addr;
    delete mem_W_Size;
    delete mem_R_Addr;
    delete reg_W_Name;
    delete reg_W_Size;
}

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
