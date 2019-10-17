#include "Assembler.h"

using namespace std;

/**************
 * Construtor *
 *************/
Assembler::Assembler(string caminho_do_arquivo) {
    this->file_path = caminho_do_arquivo;          // file_path da instancia recebe o file_path informado ao instanciar o objeto
    this->pc_codigo_fonte = 1;
    this->pc_pre_processado = 1;
    this->flag_salva_linha = 1;
    this->tabela_de_simbolos = new TabelaDeSimbolos();
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
    // Chama a funcao da passagem zero
    this->passagemZero();

    // Chama a funcao da primeira passagem de fato
    this->passagemUm("Codigo Pre-Processado.txt");
}


/**********************************************
 * Funcao que le o arquivo com o codigo fonte *
 *********************************************/
void Assembler::passagemZero() {
    int i, j;

    ofstream preProcessado;
    ifstream codigoFonte(this->file_path);           // Abre o arquivo com o codigo fonte

    this->criaListas();//cria lista de OPcodes e Diretivas

    // Verifica se o arquivo com o codigo fonte foi, de fato, aberto
    if(codigoFonte.is_open()){
        preProcessado.open("Codigo Pre-processado.txt");    // Cria o arquivo texto que contera o codigo pre-processado

        while(!codigoFonte.eof()){
            getline(codigoFonte, this->line);        // Le cada linha do codigo fonte

            this->padrao();                   // Chama a funcao para retirar comentarios
            this->pc_codigo_fonte++;                              // Incrementa o contador de progama a cada linha do codigo fonte

            // Coloca cada palavra em um vetor de palavras.
            istringstream iss(this->line);
            while (iss >> this->line)
            {
                vetor_palavras.push_back(this->line);
            }

            // Cria lista temporaria de rotulos com EQU
            for (i = 0; i < vetor_palavras.size(); i++)
            {
                if (vetor_palavras.at(i) == "EQU")
                {
                    this->apoio = vetor_palavras.at(i - 1);
                    this->apoio = this->apoio.substr(0,this->apoio.find(":"));
                    lista_temp.push_back(this->apoio);
                    lista_temp_valor.push_back(vetor_palavras.at(i + 1));
                }
            }

            // Procura valor label de EQU e substitui
            for (i = 0; i < vetor_palavras.size(); i++)
            {
                for (j = 0; j < lista_temp.size(); j++)
                {
                    if(vetor_palavras.at(i) == lista_temp.at(j))
                    {
                        vetor_palavras.at(i) = lista_temp_valor.at(j);
                    }
                }
            }


			//trata o caso de achar uma virgula

			for (i = 0; i < vetor_palavras.size(); i++)
			{
				if (vetor_palavras.at(i).find(",") != std::string::npos)
				{
					apoio = vetor_palavras.at(i).substr(vetor_palavras.at(i).find(",") + 1, vetor_palavras.at(i).size()-1);
					vetor_palavras.at(i) = vetor_palavras.at(i).substr(0, vetor_palavras.at(i).find(","));
					vetor_palavras.push_back(apoio);
				}

			}
			


            // Apaga a linha
            this->apoio.clear();
			

            //  Recria linha com valores substituidos
            for (i = 0; i < vetor_palavras.size(); i++) {
                this->apoio += vetor_palavras.at(i) + " ";
            }

            // Logica para salvar a linha correta no arquivo com o codigo pre-processado
            if (this->apoio.find("EQU") !=std::string::npos) {
                this->flag_salva_linha = 0;
            }
            if (this->apoio.find("IF") != std::string::npos) {
                this->flag_salva_linha = 0;
            }
            if (this->apoio.find("IF 0") != std::string::npos) {
                this->flag_salva_linha = -1;
            }

            // Escreve no arquivo pre-processado se a linha nao tem IF/EQU ou se a linha anterior nao era IF 0
            if (this->flag_salva_linha == 1) {

                if (vetor_palavras.size() > 1) {
                    this->saida = this->apoio + "\n";
                    //this->pc_pre_processado++;
                }
                else if (this->apoio.find(":") != std::string::npos) {
                    this->saida = this->apoio;
                }
                else {
                    this->saida = this->apoio + "\n";
                    //this->pc_pre_processado++;
                }

                // Verifica se o arquivo com o codigo pre-processado foi aberto
                if(preProcessado.is_open()) {
                    preProcessado << this->saida;        // Escreve a linha certa no arquivo pre-processado
                }
            }

            this->flag_salva_linha++;

            if (this->flag_salva_linha > 1) {
                this->flag_salva_linha = 1;
            }

            vetor_palavras.clear();

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
void Assembler::padrao() {
    int i;

    this->line = this->line.substr(0, this->line.find(";"));       // Retira os comentarios

    for (i = 0; i < this->line.length(); i++) {
        this->line.at(i) = toupper(this->line.at(i));         // Coloca a linha em maiusculo
    }
}


/*******************************************************************
 * Funcao que cria e escreve as Listas de OPcode e de Diretivas *
 ******************************************************************/
void Assembler::criaListas() {
    this->lista_de_diretivas.push_back("SECTION");
    this->lista_de_diretivas.push_back("TEXT");
    this->lista_de_diretivas.push_back("DATA");
    this->lista_de_diretivas.push_back("SPACE");
    this->lista_de_diretivas.push_back("EQU");
    this->lista_de_diretivas.push_back("IF");
    this->lista_de_opcode.push_back("ADD");
    this->lista_de_opcode.push_back("SUB");
    this->lista_de_opcode.push_back("MULT");
    this->lista_de_opcode.push_back("DIV");
    this->lista_de_opcode.push_back("JMP");
    this->lista_de_opcode.push_back("JMPN");
    this->lista_de_opcode.push_back("JMPP");
    this->lista_de_opcode.push_back("JMPZ");
    this->lista_de_opcode.push_back("COPY");
    this->lista_de_opcode.push_back("LOAD");
    this->lista_de_opcode.push_back("STORE");
    this->lista_de_opcode.push_back("INPUT");
    this->lista_de_opcode.push_back("OUTPUT");
    this->lista_de_opcode.push_back("STOP");
}


/***********************************
 * Funcao que cria a tabela de uso *
 **********************************/
void Assembler::criaTabeladeUso(string linha)
{
    int i;
    string word;
    istringstream iss(linha);
    while (iss >> word)
    {
        for (i = 0; i < this->lista_de_opcode.size(); i++)
        {
            if (word == this->lista_de_opcode.at(i)) {
                i = 20;
                break;
            }
        }

        if (i == 20)
        {
            break;
        }

        for (i = 0; i < this->lista_de_diretivas.size(); i++)
        {
            if (word == this->lista_de_diretivas.at(i)) {
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
void Assembler::passagemUm(string preProcessado){
    int i = 0, posicaotabela = 0;
	this->Linhacolunacontador = 0;
    ifstream codigoPreProcessado("Codigo Pre-processado.txt");     // Abre o arquivo que contem o codigo pre-processado
    ofstream codigoObjeto("Codigo Objeto.txt");     // Abre o arquivo texto que contera o codigo objeto

    // Pega cada palavra separada por espaco
    while (getline(codigoPreProcessado, this->line)) {
        this->vetor_palavras.clear();
        istringstream iss(this->line);
        while (iss >> this->apoio)
        {
            this->vetor_palavras.push_back(this->apoio);             // Coloca cada palavra no vetor de palavras
        }

        checaMneumonico(&posicaotabela);        // Chama a funcao que checa cada mneumonico
        this->pc_pre_processado++;

    }

    codigoPreProcessado.close();   // Fecha o arquivo com o codigo pre-processado

    this->saida.clear();
    // Coloca os opcodes no codigo objeto
    for (i = 0; i < opcodes.size(); i++) {

        saida += opcodes.at(i) + " ";
    }

    // Erro de secao TEXT faltante
    if(!section_text_present){
        // todo - ter certeza de qual o tipo desse erro
        cout << "Erro semantico???: Secao TEXT faltante" << endl;
    }
    cout << saida;
    codigoObjeto << saida;
    codigoObjeto.close();          // Fecha o arquivo com o codigo fonte

}


/*************************************************
 * Funcao que checa se a palavra eh um mneumonico *
 ************************************************/
void Assembler::checaMneumonico(int *posicaotabela) {
    int i,j;

    for (i = 0; i < this->vetor_palavras.size(); i++) {
		
		// Reconhecimento dos campos TEXT e DATA
		if (this->vetor_palavras.at(i) == "SECTION") {
			//this->Linhacolunacontador--;
			
			// todo - tratar o erro de ter apenas SECTION, ou seja, nao existir a posicao i+1 nessa linha
			if (this->vetor_palavras.at(i + 1) == "TEXT") {
				this->text_field_start = this->pc_pre_processado;
				section_text_present = true;
				cout << "Campo Text comeca na linha " << this->text_field_start << endl;

			}
			else if (this->vetor_palavras.at(i + 1) == "DATA") {
				this->data_field_start = this->pc_pre_processado;
				cout << "Campo Data comeca na linha " << this->data_field_start << endl;

				//this->Linhacolunacontador--;
			}
			else 
			{
				// todo - ter certeza de qual o tipo desse erro
				cout << "Linha " << pc_pre_processado << "; Erro semantico???: Campo invalido" << endl;
			}
            i++;    // pula palavra posterior (no caso TEXT ou DATA)
        }
        //if gerais
        else if (this->vetor_palavras.at(i) == "CONST") {
			this->Linhacolunacontador++;
			this->opcodes.push_back(this->vetor_palavras.at(i + 1));
            i++; // pula palavra posterior
        }
        else if (this->vetor_palavras.at(i) == "SPACE") {
			this->Linhacolunacontador++;
			// Caso de declaracao de um vetor
            if (this->vetor_palavras.size() - 1 > i) {
				this->Linhacolunacontador += stoi(this->vetor_palavras.at(i + 1))-1;
                for (j = 0; j < stoi(this->vetor_palavras.at(i + 1)); j++) {
                    this->opcodes.push_back("00");
					
                }
				i++;//Pula palavra posterior
            }
            // Caso de declaracao de uma variavel
				else 
				
					this->opcodes.push_back("00");
				
			}
        else if (this->vetor_palavras.at(i) == "ADD") {
			this->opcodes.push_back("1");           // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
			trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "SUB") {
            this->opcodes.push_back("2");           // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
			trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "MULT") {
            this->opcodes.push_back("3");           // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "DIV") {
            this->opcodes.push_back("4");           // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "JMP") {
            this->opcodes.push_back("5");           // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "JMPN") {
            this->opcodes.push_back("6");           // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "JMPP") {
            this->opcodes.push_back("7");           // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "JMPZ") {
            this->opcodes.push_back("8");           // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "COPY") {
            this->opcodes.push_back("9");           // Coloca o opcode da instrucao no vetor
            // todo - talevez tenha que mudar esse 2
//            trataErros(&i, 2);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "LOAD") {
            this->opcodes.push_back("10");          // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "STORE") {
            this->opcodes.push_back("11");          // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "INPUT") {
            this->opcodes.push_back("12");          // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "OUTPUT") {
            this->opcodes.push_back("13");          // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros
        }
        else if (this->vetor_palavras.at(i) == "STOP") {
            this->opcodes.push_back("14");          // Coloca o opcode da instrucao no vetor
			this->Linhacolunacontador++;
        }
        // if nao gerais
        // label definida
        else if (this->vetor_palavras.at(i).find(":") != std::string::npos)
        {
			//this->Linhacolunacontador--;
            this->apoio = this->vetor_palavras.at(i).substr(0, this->vetor_palavras.at(i).find(":"));
			//todo
			//transformar em endereco o this->opcodes
            tabela_de_simbolos->procuraPendencias(this->apoio, this->Linhacolunacontador, &this->opcodes);
            posicaotabela--;
			
		}
        //todo douradao
        //label nao definida
        else {


        }
        posicaotabela++;
		
    }
}


/*************************************************************
 * Funcao trata dos erros presentes do codigo pre-processado *
 ************************************************************/
void Assembler::trataErros(int *posicao_da_palavra, int n_operandos) {
	string aux;
    // Erros na quantidade de operandos
    if(n_operandos == 1)
    {
        if(this->vetor_palavras.size() - *posicao_da_palavra != 2){
            cout << "Linha " << pc_pre_processado << "; Erro sintatico: Quantidade de operandos invalida para a operacao" << endl;
        }
        else{
            // todo - dar push_back nos enderecos dos operandos, nao na string dos operandos
			aux = this->tabela_de_simbolos->procuraElemento(this->vetor_palavras.at(*posicao_da_palavra +1), this->Linhacolunacontador);
			//cout << this->vetor_palavras.at(*posicao_da_palavra + 1) << "    " << this->Linhacolunacontador << endl;
			//cout << "endereco:  "<< aux << endl << endl;
			this->opcodes.push_back(aux);
			this->Linhacolunacontador++;
            *posicao_da_palavra++;    // pula palavra posterior
        }
    }
    // todo - pensar melhor isso aqui, pois COPY é com ',' nao ' '
    else if(n_operandos == 2){
		//todo realizar o tratamento do COPY
        if(this->vetor_palavras.size() <= *posicao_da_palavra+2){
            cout << "Linha " << pc_pre_processado << "; Erro sintatico: Quantidade de operandos invalida para a operacao" << endl;
        }
        else{
            this->opcodes.push_back(this->vetor_palavras.at(*posicao_da_palavra+1));
            this->opcodes.push_back(this->vetor_palavras.at(*posicao_da_palavra+2));
            *posicao_da_palavra++;    // pula palavra posterior
            *posicao_da_palavra++;    // pula palavra posterior
        }
    }

}