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
	//void writePreProcesasdo();      // Funcao para criar e escrever o arquivo com o codigo pre-processado
	void separaPalavras(string liinha);

	vector<string> ListadeOPcode;
	vector<string> ListadeDiretivas;
    string filePath;                // Variavel para ser o caminho do arquivo de cada instancia do montador
};


#endif //TRABALHO1_ASSEMBLER_H
