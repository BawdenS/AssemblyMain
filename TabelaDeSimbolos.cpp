#include "TabelaDeSimbolos.h"
#include <iostream>
#include <vector>
#include <string>
/**************
 * Construtor *
 *************/
TabelaDeSimbolos::TabelaDeSimbolos() {
}

/**************
 * Destrutor *
 *************/
TabelaDeSimbolos::~TabelaDeSimbolos() {
}


/***********************************************************
 * Funcao que acrescenta um elemento na tabela de simbolos *
 **********************************************************/
void TabelaDeSimbolos::elementoDefinido(string nome, int posicao) {
    vector<int> pendencia;
    pendencia.push_back(-1);
    this->ListadeNomes.push_back(nome);
    this->Endereco.push_back(posicao);
    this->Definido.push_back(true);
    this->ListadePendencias.push_back(pendencia);
}


/***********************
 * Funcao que faz algo *
 **********************/
void TabelaDeSimbolos::elementoNaoDefinido(string nome, int posicao) {
    vector<int> pendencia;
    pendencia.push_back(-1);
    this->ListadeNomes.push_back(nome);
    this->Endereco.push_back(posicao);
    this->Definido.push_back(true);
    this->ListadePendencias.push_back(pendencia);
}


/*********************************************************
 * Funcao que procura string atual na tabela de simbolos *
 ********************************************************/
string TabelaDeSimbolos::procuraElemento(string nome, int posicao) {
    int i;
    int numero = 0;
    bool igual = false;

    for (i=0;i < this->ListadeNomes.size();i++)
    {
        if (nome == this->ListadeNomes.at(i)) {
            igual = true;
            numero = i; // posicao da lista
        }
    }
    if (igual == true) {
		//se o nome ja esta na tabela e esta definido entao esse if sera verdadeiro
		if (this->Definido.at(numero) == true) {
		//todo conferir erro por aqui retornando o valor errado ou o valor armazenado esta errado
		//se ter erro eh por aqui
		// retornar endereco de variavel ja definido
			return to_string(this->Endereco.at(numero));
		}
		else {
		//retorna -1 e adiciona na lista de pendencia algo a ser resolvido em outra funçao
			this->ListadePendencias.at(numero).push_back(posicao);
			return "-1";
		}
    }
    else {
		vector<int> pendencias;
		pendencias.push_back(posicao);
		this->ListadeNomes.push_back(nome);
		this->ListadePendencias.push_back(pendencias);
		this->Endereco.push_back(0);
		this->Definido.push_back(false);
		//cout << nome << endl  << endl;
		return "-1";
    }

}


/***********************
 * Funcao que Procura alguma pendencia de endereco no codigo *
 **********************/
//todo
//transformar em ponteiro vector <string> * opcodes
void TabelaDeSimbolos::procuraPendencias(string nome, int posicao, vector<string> opcodes) {
    int i;
    int numero = 0;
    bool igual = false;
    for (i = 0; i < this->ListadeNomes.size(); i++)
    {
        if (nome == this->ListadeNomes.at(i)) {
            igual = true;
            numero = i; // salva posicao da lista
        }
    }
    if (igual == true) {
		//todo conferir erro daqui
		//se ter erro deve ser por aqui
        //Resolve pendencia
		while (!this->ListadePendencias.at(numero).empty()) {
			opcodes.at(this->ListadePendencias.at(numero).back()) = this->Endereco.at(numero);
			this->ListadePendencias.at(numero).pop_back();
		}
		
        //vector<string>
    }
    
        //Adciona novo elemento
        this->elementoDefinido(nome, posicao);
		

}