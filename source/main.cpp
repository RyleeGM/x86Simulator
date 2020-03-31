//
//  main.cpp
//  x86Simulator
//
//  @Author - Rylee Mattingly
//  @Version - 0.0.0
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool header = 1;

int main(){

    //MARK: Variables
    ifstream test;          //Test File.
    bool header = true;     //Tells if header is present in file.
    string nextString;      //Next String to be compared against.
    
    
    //MARK: File Load and Setup
    //Load the test file.
    test.open("InputFiles/test.txt");

    //Check if the file failed to open.
    if(test.fail()){
        return 1;
    }
    
    //Peek to see if it is a header line.
    //Clear the line if it is the header.
    while(header){
        if(!(isdigit(test.peek()))){
            test.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            header = false;
        }
    }
    
    return 0;
}
