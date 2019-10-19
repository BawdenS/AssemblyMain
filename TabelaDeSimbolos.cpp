#include "TabelaDeSimbolos.h"

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
    this->lista_de_nomes.push_back(nome);
    this->endereco.push_back(posicao);
    this->definido.push_back(true);
    this->lista_de_pendencias.push_back(pendencia);
}


/*********************************************************
 * Funcao que procura string atual na tabela de simbolos *
 ********************************************************/
string TabelaDeSimbolos::procuraElemento(string nome, int posicao) {
    int i, numero = 0;
    bool igual = false;
	
    for (i=0;i < this->lista_de_nomes.size(); i++)
    {
        if (nome == this->lista_de_nomes.at(i)) {
			igual = true;
            numero = i;     // posicao da lista
        }
    }
    if (igual == true) {
		if (this->definido.at(numero) == true) {
			return to_string(this->endereco.at(numero));
		}
		else {
			this->lista_de_pendencias.at(numero).push_back(posicao);
			return "0";
		}
    }
    else {
		vector<int> pendencias;
		pendencias.push_back(posicao);
		this->lista_de_nomes.push_back(nome);
		this->lista_de_pendencias.push_back(pendencias);
		this->endereco.push_back(0);
		this->definido.push_back(false);
		return "0";
    }
}


/*************************************************************
 * Funcao que procura alguma pendencia de endereco no codigo *
 ************************************************************/
void TabelaDeSimbolos::procuraPendencias(string nome, int posicao, vector<string>* opcodes) {
    int i, numero = 0, auxint = 0;
    bool igual = false;

    for (i = 0; i < this->lista_de_nomes.size(); i++)
    {
        if (nome == this->lista_de_nomes.at(i)) {
            igual = true;
            numero = i; // salva posicao da lista
        }
    }
    if (igual == true) {
		while (!this->lista_de_pendencias.at(numero).empty()) {
			this->endereco.at(numero) = posicao;
			auxint = stoi(opcodes->at(this->lista_de_pendencias.at(numero).back()));
			opcodes->at(this->lista_de_pendencias.at(numero).back()) = to_string(posicao + auxint);
			this->lista_de_pendencias.at(numero).pop_back();
		}
    }
    
    //Adciona novo elemento
	this->elementoDefinido(nome, posicao);
}