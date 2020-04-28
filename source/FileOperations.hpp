//
//  FileOperations.hpp
//  x86Simulator
//
//  Created by Rylee Mattingly on 4/24/20.
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#ifndef FileOperations_hpp
#define FileOperations_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>


void remHeader(std::ifstream *file);
void remLine(std::ifstream *file);
int getNextInt(std::ifstream *file);
bool getNextbool(std::ifstream *file);

#endif /* FileOperations_hpp */
