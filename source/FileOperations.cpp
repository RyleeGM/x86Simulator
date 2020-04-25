//
//  FileOperations.cpp
//  x86Simulator
//
//  Created by Rylee Mattingly on 4/24/20.
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include "FileOperations.hpp"

using namespace std;

/**
Remove the header of a file.
@param file        Pointer to file ifstream
*/
void remHeader(std::ifstream *file){
    bool header = true;
    
    //Peek to see if it is a header line.
    //Clear the line if it is the header.
    while(header){
        if(!(isdigit(file->peek()))){
            file->ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            header = false;
        }
    }
}
