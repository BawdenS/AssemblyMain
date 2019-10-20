/*###################################################################
# Universidade de Brasilia                                          #
#      Disciplina de Software Basico - Trabalho 1                   #
#                                                                   #
#           Alunos                                  Matriculas      #
#                                                                   #
#   Fernando Sobral Nobrega                         15/0034911      #
#   Matheus Bawden Silverio Castro                  15/0141017      #
#                                                                   #
#            Compilador e Sistema Operacional usados                #
#           g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0               #
#                                                                   #
###################################################################*/

#include "Assembler.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    string filepath = argv[1];

	Assembler* assembler = new Assembler(filepath);
	assembler->assemble();

	return 0;
}