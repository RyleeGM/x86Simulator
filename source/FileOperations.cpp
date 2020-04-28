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

/**
Remove the current line of the file.
@param file    pointer to the file ifstream
*/
void remLine(ifstream *file){
    file->ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 Get the next int from the file.
 @param file pointer to the file ifstream
 */
int getNextInt(ifstream *file){
    
    //Check to see if the next char is a space.
    while(file->peek() == ' ' || file->peek() == '\n'){
        file->ignore(1);
    }
    char temp[10];
    file->get(temp, 10, ' ');
    return stoi(string(temp));
}

/**
 Get the next address.
 @param file pointer to the file stream
 */
unsigned long long int getNextAddr(ifstream *file){
    //Check to see if the next char is a space.
    while(file->peek() == ' ' || file->peek() == '\n'){
        file->ignore(1);
    }
    char temp[20];
    file->get(temp, 20, ' ');
    return (uint64_t)stoull(string(temp));
}

/**
Get the next bool from the file.
@param file pointer to the file ifstream
*/
bool getNextbool(ifstream *file){
    
    while(file->peek() == ' ' || file->peek() == '\n'){
        file->ignore(1);
    }
    char temp[3];
    file->get(temp, 3, ' ');
    return (stoi(temp) == 0 ? false : true);
}
