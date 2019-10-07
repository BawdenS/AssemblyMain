#ifndef TRABALHO1_ASSEMBLER_H
#define TRABALHO1_ASSEMBLER_H

#include <string>
#include <vector>
#include "TabelaDeSimbolo.h"
using namespace std;

class Assembler {
public:
    Assembler(string filePath);     // Construtor
    ~Assembler();                   // Destrutor
    void assemble();                // Funcao que monta o programa

private:
    void passagemZero();                // Funcao para leitura do codigo fonte
    void padrao();         // Funcao para retirar comentarios e ignorar letras maiusculas/minusculas
	void CriaListas();				    // Funcao para criar listas de OPcode e Diretivas de maneira vector de strings
	void CriaTabeladeUso(string);	    // Funcao para criar lista de usos que l? de palavra em palavra comparando com a ListadeOPcode e Lista de Diretivas
	void passagemUm(string);      // Funcao que separa inicia a primeira passagem de fato
	void checaMneumonico(int *);     // Funcao para tratamento de erros na linha
	TabelaDeSimbolo* tabeladesimbolo;
	vector<string> ListadeOPcode;
	vector<string> ListadeDiretivas;
    vector<string> listatemp;
    vector<string> listatempvalor;
    vector<string> vetorpalavras;
	vector<string> opcodes;
    string filePath, word[4], line, saida, apoio;
    int text_field_start, data_field_start, pc_codigo_fonte, pc_pre_processado,
        line_opcode, flagsalvalinha;
};


#endif //TRABALHO1_ASSEMBLER_H

