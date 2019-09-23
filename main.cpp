#include "Assembler.h"

using namespace std;

int main(int argc, char* arv[]) {

    Assembler* assembler = new Assembler("teste.txt");
    assembler->Assemble();

    return 0;
}