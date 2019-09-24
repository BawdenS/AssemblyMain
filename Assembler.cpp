#include "Assembler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
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
	this->CriaListas();//cria lista de OPcodes e Diretivas
    // Verifica se o arquivo com o codigo fonte foi, de fato, aberto
    if(codigoFonte.is_open()){
        preProcessado.open("Codigo Pre-processado.txt");    // Cria o arquivo texto que contera o codigo pre-processado

        while(!codigoFonte.eof()){
            getline(codigoFonte, line);        // Le cada linha do codigo fonte
            //cout << line << endl;                     // Printa a linha lida

            this->padrao(line);                   // Chama a funcao para retirar comentarios
            cout << line << endl;            // Printa a linha lida sem comentarios e em maiusculo
			

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


/*******************************************************************
 * Funcao que cria e escreve as Listas de OPcode e de Diretivas *
 ******************************************************************/
void Assembler::CriaListas() {
	this->ListadeDiretivas.push_back("SECTION");
	this->ListadeDiretivas.push_back("TEXT");
	this->ListadeDiretivas.push_back("DATA");
	this->ListadeDiretivas.push_back("SPACE");
	this->ListadeDiretivas.push_back("EQU");
	this->ListadeDiretivas.push_back("IF");
	this->ListadeOPcode.push_back("ADD");
	this->ListadeOPcode.push_back("SUB");
	this->ListadeOPcode.push_back("MULT");
	this->ListadeOPcode.push_back("DIV");
	this->ListadeOPcode.push_back("JMP");
	this->ListadeOPcode.push_back("JMPN");
	this->ListadeOPcode.push_back("JMPP");
	this->ListadeOPcode.push_back("JMPZ");
	this->ListadeOPcode.push_back("COPY");
	this->ListadeOPcode.push_back("LOAD");
	this->ListadeOPcode.push_back("STORE");
	this->ListadeOPcode.push_back("INPUT");
	this->ListadeOPcode.push_back("OUTPUT");
	this->ListadeOPcode.push_back("STOP");
}

void Assembler::CriaTabeladeUso(string linha) 
{
	int i;
	string word;
	istringstream iss(linha);
	while (iss >> word) 
	{
		for (i = 0; i < this->ListadeOPcode.size();i++) 
		{
			if (word == this->ListadeOPcode.at(i)) {
				i = 20;
				break;
			}
		}
		if (i == 20) 
		{
			break;
		}
		for (i = 0; i < this->ListadeDiretivas.size(); i++)
		{
			if (word == this->ListadeDiretivas.at(i)) {
				i = 20;
				break;
			}
		}
		if (i == 20)
		{
			break;
		}
		//Se chegar a esta linha a palavra lida com certeza nao é um elemento da tabela de Diretivas ou de OPcode
		//Precisamos verificar se o elemento têm ":", se possuir ":" é um elemento já definido e precisamos salvar a posicao atual
		//posicao atual estara no elemento this->posicao e sera salvado no vetor this->endereco.push_back() e será necessario
		//conferir a lista de pendencias para esse elemento
		
		//se for

	}

}