#include"TabelaDeSimbolo.h"
#include <iostream>


/**************
 * Construtor *
 *************/
TabelaDeSimbolo::TabelaDeSimbolo() {
}


void TabelaDeSimbolo::ElementoDefinido(string nome, int posicao) {
	vector<int> pendencia;
	pendencia.push_back(-1);
	this->ListadeNomes.push_back(nome);
	this->Endereco.push_back(posicao);
	this->Definido.push_back(true);
	this->ListadePendencias.push_back(pendencia);
}

void TabelaDeSimbolo::ElementoNaoDefinido(string nome, int posicao) {
	vector<int> pendencia;
	pendencia.push_back(-1);
	this->ListadeNomes.push_back(nome);
	this->Endereco.push_back(posicao);
	this->Definido.push_back(true);
	this->ListadePendencias.push_back(pendencia);
}








void TabelaDeSimbolo::ProcuraElemento(string nome, int posicao) {
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

void TabelaDeSimbolo::ProcuraPendencias(string nome, int posicao) {
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
		this->ElementoDefinido(nome, posicao);
	}

}
