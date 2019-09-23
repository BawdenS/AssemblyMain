#ifndef TRABALHO1_ASSEMBLER_H
#define TRABALHO1_ASSEMBLER_H

#include <string>

using namespace std;

class Assembler {
public:
    Assembler(string filePath);     // Construtor
    ~Assembler();                   // Destrutor
    void Assemble();                // Funcao que monta o programa

private:
    void readFile();                // Funcao para leitura do codigo fonte
    void Padrao(string& linha);     // Funcao para retirar comentarios e ignorar letras maiusculas/minusculas
    string filePath;                // Variavel para ser o caminho do arquivo de cada instancia do montador
};


#endif //TRABALHO1_ASSEMBLER_H
