#ifndef TRABALHO1_ASSEMBLER_H
#define TRABALHO1_ASSEMBLER_H

#include <string>
#include <vector>
using namespace std;

class Assembler {
public:
    Assembler(string filePath);     // Construtor
    ~Assembler();                   // Destrutor
    void assemble();                // Funcao que monta o programa

private:
    void passagemZero();                // Funcao para leitura do codigo fonte
    void padrao(string& linha);     // Funcao para retirar comentarios e ignorar letras maiusculas/minusculas
	void CriaListas();				// Funcao para criar listas de OPcode e Diretivas de maneira vector de strings
	void CriaTabeladeUso(string);	// Funcao para criar lista de usos que l? de palavra em palavra comparando com a ListadeOPcode e Lista de Diretivas
	void separaPalavras(string linha, string* palavras, int* opcode);          // Funcao que separa as palavras de cada linha
	void checaMneumonico(string* palavras, int* opcode);

	vector<string> ListadeOPcode;
	vector<string> ListadeDiretivas;
	vector<int> opcodes;
    string filePath;                // Variavel para ser o caminho do arquivo de cada instancia do montador
};


#endif //TRABALHO1_ASSEMBLER_H

