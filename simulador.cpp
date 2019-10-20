#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main(int Nargumentos, char*Argumentos[])
{
	int i, Acumulador = 0;
	string NomeArquivo = Argumentos[1], Linha, Word;
	vector<int> Unidade;
	ifstream Leitura;

	Leitura.open(NomeArquivo.c_str());
	if (!Leitura.is_open()) 
	{
		cerr << " Nao eh possivel abrir arquivo" << endl;
		exit(1);
	}

	while (getline(Leitura,Linha)) 
	{
		istringstream iss(Linha);
		while (iss >> Word) 
		{
			Unidade.push_back(stoi(Word));
		}
	}
	for (i = 0; i < Unidade.size();i++) {
		//ADD
		if (Unidade.at(i) == 1){
			Acumulador += Unidade.at(Unidade.at(i + 1));
			i++;
			cout << "Acumulador: " << Acumulador << endl;
		}
		//SUB
		else if (Unidade.at(i) == 2) {
			Acumulador -= Unidade.at(Unidade.at(i + 1));
			i++;
			cout << "Acumulador: " << Acumulador << endl;
		}
		//MULT
		else if (Unidade.at(i) == 3) {
			Acumulador *= Unidade.at(Unidade.at(i + 1));
			i++;
			cout << "Acumulador: " << Acumulador << endl;
		}
		//DIV
		else if (Unidade.at(i) == 4) {
			Acumulador /= Unidade.at(Unidade.at(i + 1));
			i++;
			cout << "Acumulador: " << Acumulador << endl;
		}
		//JMP
		else if (Unidade.at(i) == 5) {
			i = Unidade.at(i++);
			cout << "Acumulador: " << Acumulador << endl;
		}
		//JMPN
		else if (Unidade.at(i) == 6) {
			if (Acumulador < 0) {
				i = Unidade.at(i++);
			}
			else 
			{
				cout << "Acumulador: " << Acumulador << endl;
				i++;
			}
		}
		//JMPP
		else if (Unidade.at(i) == 7) {
			if (Acumulador > 0) {
				i = Unidade.at(i++);
			}
			else
			{
				cout << "Acumulador: " << Acumulador << endl;
				i++;
			}
		}
		//JMPZ
		else if (Unidade.at(i) == 8) {
			if (Acumulador == 0) {
				i = Unidade.at(i++);
			}
			else
			{
				cout << "Acumulador: " << Acumulador << endl;
				i++;
			}
		}
		//COPY
		else if (Unidade.at(i) == 9) {
			Unidade.at(Unidade.at(i + 2)) = Unidade.at(Unidade.at(i + 1));
			i++;
			i++;
			cout << "Acumulador: " << Acumulador << endl;
		}
		//LOAD
		else if (Unidade.at(i) == 10) {
			Acumulador = Unidade.at(Unidade.at(i + 1));
			i++;
			cout << "Acumulador: "<< Acumulador << endl;
		}
		//STORE
		else if (Unidade.at(i) == 11) {
			Unidade.at(Unidade.at(i + 1)) = Acumulador;
			cout << "Memoria em: "<< Unidade.at(i + 1) << " Alterada para: " << Acumulador << endl;
			cout << "Acumulador: " << Acumulador << endl;
		}
		//INPUT
		else if (Unidade.at(i) == 12) {
			cout << "Favor Inserir Valor numerico: "<< endl;
			cin >> Unidade.at(Unidade.at(i + 1));
			cout << "Memoria em: " << Unidade.at(i + 1) << " Alterada para: " <<  Unidade.at(Unidade.at(i + 1)) << endl;
			cout << "Acumulador: " << Acumulador << endl;
            i++;
        }
		//OUTPUT
		else if (Unidade.at(i) == 13) {
			cout << "O Valor de Saida eh: " << Unidade.at(Unidade.at(i + 1)) << endl;
			i++;
		}
		//STOP
		else if (Unidade.at(i) == 14) {
			cout << "Fim do programa" <<endl;
			return 0;
		}
	}
	Leitura.close();
	return 0;
}