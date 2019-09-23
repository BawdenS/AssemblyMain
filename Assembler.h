#ifndef TRABALHO1_ASSEMBLER_H
#define TRABALHO1_ASSEMBLER_H

#include <string>

using namespace std;

class Assembler {
public:
    Assembler(/*string filePath*/);         // Construtor
    ~Assembler();                           // Destrutor
    static void readFile();

private:
    //void readFile();
    //static Assembler* assembler;
};


#endif //TRABALHO1_ASSEMBLER_H
