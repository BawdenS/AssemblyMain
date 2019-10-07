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
void TabelaDeSimbolos::procuraElemento(string nome, int posicao) {
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


    }
    else {

    }

}


/***********************
 * Funcao que faz algo *
 **********************/
void TabelaDeSimbolos::procuraPendencias(string nome, int posicao) {
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
        //Resolve pendencia
        this->ListadePendencias.at(numero);
        //vector<string>
    }
    else {
        //Adciona novo elemento
        this->elementoDefinido(nome, posicao);
    }

}