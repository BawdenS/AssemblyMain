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
    this->passagemZero();           // Chama a funcao de leitura do codigo
}


/**********************************************
 * Funcao que le o arquivo com o codigo fonte *
 *********************************************/
void Assembler::passagemZero() {
    
	int i, pc = 1, opcode;
    string line, words[4];
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
            cout << pc << " " << line << endl;       // Printa o contador de programa e a linha lida sem comentarios e em maiusculo
            pc++;                                    // Incrementa o contador de progama

            this->separaPalavras(line, words, &opcode);             // Chama a funcao que separa cada palavras

            //cout << "OPCODE = " << opcode << endl;    // Isso aqui printa nas linhas de deritivas sem opcode tbm

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
		//Se chegar a esta linha a palavra lida com certeza nao eh um elemento da tabela de Diretivas ou de OPcode
		//Precisamos verificar se o elemento tem ":", se possuir ":" eh um elemento ja definido e precisamos salvar a posicao atual
		//posicao atual estara no elemento this->posicao e sera salvado no vetor this->endereco.push_back() e sera necessario
		//conferir a lista de pendencias para esse elemento
		
		//se for

	}

}


/***********************************************
 * Funcao que separa as palavras de cada linha *
 **********************************************/
void Assembler::separaPalavras(string linha, string *palavras, int* opcode){
    int i = 0;
    string word;

    // Inicializa o vetor de palavras da linhas com NULL
    palavras[0] = "NULL";
    palavras[1] = "NULL";
    palavras[2] = "NULL";
    palavras[3] = "NULL";

    // Pega cada palavra separada por espaco
    istringstream iss(linha);
    while (iss >> word)
    {
        //cout << " | "<< word << " | ";      // Printa cada palavra sepaarada
        palavras[i] = word;             // Coloca cada palavra no vetor de palavras
        i++;
    }

    checaMneumonico(palavras, opcode);            // Chama a funcao que checa se eh uma diretiva ou instrucao

    //cout << endl;                           // Printa uma quebra de
/*
    for(i = 0; i < 4; i++){
        cout << " ( " << palavras[i] << " ) ";
    }
    cout << endl;                           // Printa uma quebra de linha
*/
}


/*************************************************
 * Funcao que checa se a palavra eh um mneumonico *
 ************************************************/
void Assembler::checaMneumonico(string* palavras, int* opcode) {
    if(palavras[0] == "SECTION"){
        if(palavras[1] == "TEXT")
            cout << "        EH DIRETIVA SECTION TEXT!" << endl;
        else if(palavras[1] == "DATA")
            cout << "        EH DIRETIVA SECTION DATA!" << endl;
    }
    else if(palavras[0] == "SPACE"){
        *opcode = 00;

        if(palavras[1] != "NULL"){
            cout << "        EH DIRETIVA SPACE E VALOR: " << palavras[1] << " OPCODE = " << *opcode << *opcode << endl;
        }
        else{
            cout << "        EH DIRETIVA SPACE E VALOR: 1 OPCODE = " << *opcode << *opcode << endl;
        }
    }
    else if(palavras[0] == "CONST"){
        cout << "        EH DIRETIVA CONST  E VALOR: " << palavras[1] << endl;
    }
    else if(palavras[0] == "EQU"){
        cout << "        EH DIRETIVA EQU  E VALOR: " << palavras[1] << endl;
    }
    else if(palavras[0] == "IF"){
        cout << "        EH DIRETIVA IF!" << endl;
    }
    else if(palavras[0] == "ADD"){
        *opcode = 1;
        cout << "        EH INSTRUCAO ADD!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "SUB"){
        *opcode = 2;
        cout << "        EH INSTRUCAO SUB!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "MULT"){
        *opcode = 3;
        cout << "        EH INSTRUCAO MULT!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "DIV"){
        *opcode = 4;
        cout << "        EH INSTRUCAO DIV!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "JMP"){
        *opcode = 5;
        cout << "        EH INSTRUCAO JMP!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "JMPN"){
        *opcode = 6;
        cout << "        EH INSTRUCAO JMPN!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "JMPP"){
        *opcode = 7;
        cout << "        EH INSTRUCAO JMPP!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "JMPZ"){
        *opcode = 8;
        cout << "        EH INSTRUCAO JMPZ!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "COPY"){
        *opcode = 9;
        cout << "        EH INSTRUCAO COPY!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "LOAD"){
        *opcode = 10;
        cout << "        EH INSTRUCAO LOAD!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "STORE"){
        *opcode = 11;
        cout << "        EH INSTRUCAO STORE!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "INPUT"){
        *opcode = 12;
        cout << "        EH INSTRUCAO INPUT!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "OUTPUT"){
        *opcode = 13;
        cout << "        EH INSTRUCAO OUTPUT!     OPCODE = " << *opcode << endl;
    }
    else if(palavras[0] == "STOP"){
        *opcode = 14;
        cout << "        EH INSTRUCAO STOP!     OPCODE = " << *opcode << endl;
    }
    else{
        // todo - checar o ':'
        cout << "        NAO EH MNEUMONICO, EH SIMBOLO! ";
        if(palavras[1] == "SPACE"){
            *opcode = 00;
            if(palavras[2] != "NULL"){
                cout << "        EH DIRETIVA SPACE E VALOR: " << palavras[1] << " OPCODE = " << *opcode << *opcode << endl;
            }
            else{
                cout << "        EH DIRETIVA SPACE E VALOR: 1 OPCODE = " << *opcode << *opcode << endl;
            }
        }
        else if(palavras[1] == "CONST"){
            cout << "SEGUIDO DE DIRETIVA CONST E VALOR: " << palavras[2] << endl;
        }
        else if(palavras[1] == "EQU"){
            cout << "SEGUIDO DE DIRETIVA EQU  E VALOR: " << palavras[2] << endl;
        }
        else if(palavras[1] == "IF"){
            cout << "SEGUIDO DE DIRETIVA IF!" << endl;
        }
        else if(palavras[1] == "ADD"){
            *opcode = 1;
            cout << "        EH INSTRUCAO ADD!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "SUB"){
            *opcode = 2;
            cout << "        EH INSTRUCAO SUB!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "MULT"){
            *opcode = 3;
            cout << "        EH INSTRUCAO MULT!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "DIV"){
            *opcode = 4;
            cout << "        EH INSTRUCAO DIV!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "JMP"){
            *opcode = 5;
            cout << "        EH INSTRUCAO JMP!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "JMPN"){
            *opcode = 6;
            cout << "        EH INSTRUCAO JMPN!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "JMPP"){
            *opcode = 7;
            cout << "        EH INSTRUCAO JMPP!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "JMPZ"){
            *opcode = 8;
            cout << "        EH INSTRUCAO JMPZ!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "COPY"){
            *opcode = 9;
            cout << "        EH INSTRUCAO COPY!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "LOAD"){
            *opcode = 10;
            cout << "        EH INSTRUCAO LOAD!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "STORE"){
            *opcode = 11;
            cout << "        EH INSTRUCAO STORE!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "INPUT"){
            *opcode = 12;
            cout << "        EH INSTRUCAO INPUT!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "OUTPUT"){
            *opcode = 13;
            cout << "        EH INSTRUCAO OUTPUT!     OPCODE = " << *opcode << endl;
        }
        else if(palavras[1] == "STOP"){
            *opcode = 14;
            cout << "        EH INSTRUCAO STOP!     OPCODE = " << *opcode << endl;
        }
    }

}