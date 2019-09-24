#include"TabeladeUso.h"
#include <iostream>

void TabeladeUso::ElementoDefinido(string nome, int posicao) {
	vector<int> pendencia;
	pendencia.push_back(-1);
	this->ListadeNomes.push_back(nome);
	this->Endreco.push_back(posicao);
	this->Definido.push_back(true);
	this->ListadePendencias.push_back(pendencia);
}

void TabeladeUso::ProcuraElemento(string nome) {



}