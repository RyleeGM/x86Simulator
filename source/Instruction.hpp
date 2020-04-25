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
    int* reg_R_Name; //Optional
    int* reg_R_Size; //Optional
    //Memory Read
    int num_MemR;
    uint64_t* mem_R_Addr;   //Optional
    int* mem_R_Size;        //Optional
    //Memory Write
    int num_MemW;
    uint64_t* mem_W_Addr;   //Optional
    int* mem_W_Size;        //Optional
    //Register Write
    int num_RegW;
    int* reg_W_Name;        //Optional
    int* reg_W_Size;        //Optional
    
public:
    //Constructors
    Instruction(std::ifstream *insFile);
    
    //Destructor
    virtual ~Instruction();
    
    //Overloaded Operators
    friend std::ostream& operator<< (std::ostream& s, Instruction& ins);
    
};





#endif /* Instruction_hpp */
