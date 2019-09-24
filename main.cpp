/*###################################################################
# Universidade de Brasilia                                          #
#      Disciplina de Software Basico - Trabalho 1                   #
#                                                                   #
#           Alunos                                  Matriculas      #
#                                                                   #
#   Fernando Sobral Nobrega                         15/0034911      #
#   Matheus Bawden Silverio Castro                  15/0141017      #
#                                                                   #
#                                                                   #
###################################################################*/

#include "Assembler.h"

using namespace std;

int main(int argc, char* arv[]) {

	Assembler* assembler = new Assembler("teste.txt");
	assembler->assemble();

	return 0;
}