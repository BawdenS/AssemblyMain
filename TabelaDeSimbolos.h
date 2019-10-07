#ifndef TRABALHO1_TABELA_DE_SIMBOLOS_H
#define TRABALHO1_TABELA_DE_SIMBOLOS_H

#include <string>
#include <vector>

using namespace std;

class TabelaDeSimbolos {
public:
    TabelaDeSimbolos();
    ~TabelaDeSimbolos();
    void elementoDefinido(string nome, int posicao);    // funcao que acrescenta um elemento na tabela de simbolos
    void procuraElemento(string nome, int posicao);     // funcao que procura string atual na tabela de simbolos
    void procuraPendencias(string nome, int posicao);
    void elementoNaoDefinido(string nome, int posicao);

private:
    vector<string> ListadeNomes;
    vector<vector<int>> ListadePendencias;
    vector<int> Endereco;
    vector<bool> Definido;
};

#endif //TRABALHO1_TABELA_DE_SIMBOLOS_H