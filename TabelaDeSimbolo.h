#ifndef TABELADESIMBOLO_H
#define TABELADESIMBOLO_H

#include <string>
#include <vector>
using namespace std;

class TabelaDeSimbolo {
public:
	TabelaDeSimbolo();
	void ElementoDefinido(string, int); //funcao que acrescenta um elemento na tabela
	void ProcuraElemento(string, int); //funcao que procura string atual na tabela
	void ProcuraPendencias(string, int);
	void ElementoNaoDefinido(string, int);

private:
	vector<string> ListadeNomes;
	vector<vector<int>> ListadePendencias;
	vector<int> Endereco;
	vector<bool> Definido;
};

#endif