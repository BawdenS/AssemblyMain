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
void Assembler::Assemble() {
    this->readFile();           // Chama a funcao de leitura do codigo
}


/**********************************************
 * Funcao que le o arquivo com o codigo fonte *
 *********************************************/
void Assembler::readFile() {

    string line;
    //ifstream myFile("teste.txt");
    ifstream myFile(this->filePath);

    if(myFile.is_open()){
        while(!myFile.eof()){
            getline(myFile, line);        // Le cada linha do codigo fonte
            cout << line << endl;               // Printa a linha lida
            this->Padrao(line);             // Chama a funcao para retirar comentarios
            cout << line << endl << endl;      // Printa a linha sem comentarios
        }
        myFile.close();     // Fecha o arquivo com o codigo fonte
    }
    else
        cout << "Unable to open file" << endl;
}


/***********************************************************************
 * Funcao que retira comentarios e ignora letras maiusculas/minusculas *
 **********************************************************************/
void Assembler::Padrao(string& linha) {
    int i;

        linha = linha.substr(0, linha.find(";"));       // Retira os comentarios

    for (i = 0; i < linha.length(); i++) {
        linha.at(i) = toupper(linha.at(i));         // Coloca tudo em maiusculo
    }
}
