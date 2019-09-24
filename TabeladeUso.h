#ifndef TRABALHO1_ASSEMBLER_H
#define TRABALHO1_ASSEMBLER_H

#include <string>
#include <vector>
using namespace std;

class TabeladeUso {
public:
	void ElementoDefinido(string, int); //funcao que acrescenta um elemento na tabela
	void ProcuraElemento(string);

private:
	vector<string> ListadeNomes;
	vector<vector<int>> ListadePendencias;
	vector<int> Endreco;
	vector<bool> Definido;
};

#endif