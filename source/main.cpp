//
//  main.cpp
//  x86Simulator
//
//  @Author - Rylee Mattingly
//  @Version - 0.0.0
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include "Instruction.hpp"
#include "FileOperations.hpp"

using namespace std;

bool header = 1;

int main(){

    //MARK: Variables
    ifstream test;          //Test File.
    ifstream trace;         //Trace File.
    ifstream instructions;  //File containing instuction data.
    string nextString;      //Next String to be compared against.
    
    
    //MARK: File Load and Setup
    //Load the test file.
    test.open("InputFiles/test.txt");
    trace.open("InputFiles/trace.out");
    instructions.open("InputFiles/InstructionLatencies.txt");
    
    //Check if the file failed to open.
    if(test.fail() || trace.fail() || instructions.fail()){
        return 1;
    }
    
    remHeader(&test);
    remHeader(&trace);
    remHeader(&instructions);
    
    //Temporary Testing Region.
    
    Instruction ins [10];
    
    for(int i = 0; i < 10; i++){
        ins[i] = Instruction(&trace);
    }
    
    for(int i = 0; i < 10; i++){
        cout << ins[i] << "\n";
    }
    
    cout << ins[1].isProducer(vector<int>{1, 2, 7, 25}) << "\n";
    
    //Temporary Testing Region end.
    
    //Close files for clean exit.
    test.close();
    trace.close();
    instructions.close();
    
    return 0;
}
