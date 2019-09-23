#include "Assembler.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Assembler::Assembler() {
}


void Assembler::readFile() {

    string line;
    ifstream myFile;//("bin.asm.txt");
    myFile.open("bin.asm.txt");

    if(myFile.is_open()){
        while(!myFile.eof()){
            getline(myFile, line);      // Scans each line
            cout << line << endl;
        }
        myFile.close();     // Close file
    }
    else
        cout << "Unable to open file" << endl;
}


Assembler::~Assembler() {
}
