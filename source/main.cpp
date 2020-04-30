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

int main(int argc, char *argv[]){

    //MARK: Variables
    ifstream trace;         //Trace File.
    ifstream latencies;     //File containing instuction data.
    ifstream settings;      //File containing the settings.
    
    string traceName = "InputFiles/";
    traceName += argv[1];
    string latencyName = "InputFiles/";
    latencyName += argv[2];
    string settingsName = "InputFiles/";
    settingsName += argv[3];
    
    //MARK: File Load and Setup
    //Load Files.
    trace.open(traceName);
    latencies.open(latencyName);
    settings.open(settingsName);
    
    //Check if any file failed to open.
    if(trace.fail() || latencies.fail() || settings.fail()){
        cerr << "A File Failed to Open" << "\n" << "Exiting";
        return 1;
    }
    
    //Remove file headers
    remHeader(&trace);
    remHeader(&latencies);
    remHeader(&settings);
    
    SimState simulation = SimState(&latencies, &settings);
    
    simulation.manager(&trace);
    simulation.report();
    
    //Close files for clean exit.
    trace.close();
    latencies.close();
    settings.close();
    
    return 0;
}
