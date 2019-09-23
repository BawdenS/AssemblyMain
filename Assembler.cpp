#include "Assembler.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**************
 * Construtor *
 *************/
Assembler::Assembler(string filePath) {
    this->filePath = filePath;          // filePath da instancia recebe o filePath informado ao instanciar o objeto
}


/*************
 * Destrutor *
 ************/
Assembler::~Assembler() {
}


/***********************************
 * Funcao que monta o codigo fonte *
 **********************************/
void Assembler::assemble() {
    this->readFile();           // Chama a funcao de leitura do codigo
}


/**********************************************
 * Funcao que le o arquivo com o codigo fonte *
 *********************************************/
void Assembler::readFile() {
    int i;
    string line;
    ofstream preProcessado;
    ifstream codigoFonte(this->filePath);           // Abre o arquivo com o codigo fonte

    // Verifica se o arquivo com o codigo fonte foi, de fato, aberto
    if(codigoFonte.is_open()){
        preProcessado.open("Codigo Pre-processado.txt");    // Cria o arquivo texto que contera o codigo pre-processado

        while(!codigoFonte.eof()){
            getline(codigoFonte, line);        // Le cada linha do codigo fonte
            cout << line << endl;                     // Printa a linha lida

            this->padrao(line);                   // Chama a funcao para retirar comentarios
            cout << line << endl << endl;            // Printa a linha lida sem comentarios e em maiusculo


            // Verifica se o arquivo com o codigo pre-processado foi aberto
            if(preProcessado.is_open()) {
                preProcessado << line << endl;       // Escreve a linha no arquivo de codigo pre-processado
            }
            //this->writePreProcesasdo();
        }
        codigoFonte.close();     // Fecha o arquivo com o codigo fonte
        preProcessado.close();   // Fecha o arquivo com o codigo pre-processado
    }
    else
        cout << "Unable to open file" << endl;
}


/***********************************************************************
 * Funcao que retira comentarios e ignora letras maiusculas/minusculas *
 **********************************************************************/
void Assembler::padrao(string& linha) {
    int i;

    linha = linha.substr(0, linha.find(";"));       // Retira os comentarios

    for (i = 0; i < linha.length(); i++) {
        linha.at(i) = toupper(linha.at(i));         // Coloca a linha em maiusculo
    }
}


/*******************************************************************
 * Funcao que cria e escreve o arquivo com o codigo pre-processado *
 ******************************************************************/
/*void Assembler::writePreProcesasdo() {
    ofstream preProcessado;//("Codigo Pre-processado.txt");
    preProcessado.open("Codigo Pre-processado.txt");

    if(preProcessado.is_open()){
        preProcessado << "asd" <<  endl;
    }

    preProcessado.close();   // Fecha o arquivo com o codigo pre-processado

}
*/