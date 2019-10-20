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

O trabalho foi em c++ para rodar o LINUX, Ubuntu 7.4.0-1ubuntu1~18.04.1, consiste de um Programa montador e um Programa simulador que recebem por argumento os arquivos .asm e um arquivo .obj respectivamente.
Os comandos necessários são:
g++ main.cpp Assembler.cpp TabelaDeSimbolos.cpp -o montador
./montador exemplo.asm
g++ simulador.cpp -o simulador
./simulador exemplo.obj
