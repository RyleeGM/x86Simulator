//
//  main.cpp
//  x86Simulator
//
//  @Author - Rylee Mattingly
//  @Version - 0.0.0
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include "SimState.hpp"

using namespace std;

bool header = 1;

int main(){

    //MARK: Variables
    ifstream test;          //Test File.
    ifstream trace;         //Trace File.
    ifstream latencies;     //File containing instuction data.
    ifstream settings;      //File containing the settings.
    
    
    //MARK: File Load and Setup
    //Load Files.
    test.open("InputFiles/test.txt");
    trace.open("InputFiles/trace.out");
    latencies.open("InputFiles/InstructionLatencies.txt");
    settings.open("InputFiles/settings.txt");
    
    //Check if any file failed to open.
    if(test.fail() || trace.fail() || latencies.fail() || settings.fail()){
        cerr << "A File Failed to Open" << "\n" << "Exiting";
        return 1;
    }
    
    //Remove file headers
    remHeader(&test);
    remHeader(&trace);
    remHeader(&latencies);
    remHeader(&settings);
    
    SimState simulation = SimState(&latencies, &settings);
    
    simulation.manager(&trace);
    simulation.report();
    
    //Close files for clean exit.
    test.close();
    trace.close();
    latencies.close();
    
    return 0;
}
