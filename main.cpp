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

using namespace std;

int main(){

    //Load the test file for reading capabilities.
    ifstream test;
    test.open("test.txt");

    int x, y;

    cout << "Is this working" << "\n";

    test >> x >> y;

    cout << x << y;


    return 10;
}
