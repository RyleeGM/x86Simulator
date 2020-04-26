//
//  main.cpp
//  x86Simulator
//
//  @Author - Rylee Mattingly
//  @Version - 0.0.0
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include "SimState.hpp"
#include "FileOperations.hpp"

using namespace std;

bool header = 1;

int main(){

    //MARK: Variables
    ifstream test;          //Test File.
    ifstream trace;         //Trace File.
    ifstream latencies;     //File containing instuction data.
    string nextString;      //Next String to be compared against.
    
    
    //MARK: File Load and Setup
    //Load Files.
    test.open("InputFiles/test.txt");
    trace.open("InputFiles/trace.out");
    latencies.open("InputFiles/InstructionLatencies.txt");
    
    //Check if any file failed to open.
    if(test.fail() || trace.fail() || latencies.fail()){
        cerr << "A File Failed to Open" << "\n" << "Exiting";
        return 1;
    }
    
    //Remove file headers
    remHeader(&test);
    remHeader(&trace);
    remHeader(&latencies);
    
    //Temporary Testing Region.
    
    SimState simulator = SimState(&latencies);
    
    //Temporary Testing Region end.
    
    //Close files for clean exit.
    test.close();
    trace.close();
    latencies.close();
    
    return 0;
}
