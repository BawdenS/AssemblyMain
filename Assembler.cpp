#include "Assembler.h"

using namespace std;

/**************
 * Construtor *
 *************/
Assembler::Assembler(string caminho_do_arquivo) {
    this->file_path = caminho_do_arquivo;          // file_path da instancia recebe o file_path informado ao instanciar o objeto
    this->file_pre = this->file_path.substr(0,this->file_path.find(".asm"))+".pre";
    this->file_obj = this->file_path.substr(0, this->file_path.find(".asm"))+".obj";
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
    this->passagemUm();
	int i;
	for (i = 0; i < this->tabela_de_simbolos->definido.size();i++) {
		if (this->tabela_de_simbolos->definido.at(i) == false) {
			cout <<" Erro semantico rotulo nao definido" << endl;
		}
	}
}


/**********************************************
 * Funcao que le o arquivo com o codigo fonte *
 *********************************************/
void Assembler::passagemZero() {
    int i, j;

    ofstream preProcessado;
    ifstream codigoFonte(this->file_path);           // Abre o arquivo com o codigo fonte

    this->criaListas();     // cria lista de OPcodes e Diretivas

    // Verifica se o arquivo com o codigo fonte foi, de fato, aberto
    if(codigoFonte.is_open()){
        preProcessado.open(this->file_pre);    // Cria o arquivo texto que contera o codigo pre-processado

        while(!codigoFonte.eof()){
            getline(codigoFonte, this->line);        // Le cada linha do codigo fonte

            this->padrao();                   // Chama a funcao para retirar comentarios
            this->pc_codigo_fonte++;          // Incrementa o contador de progama a cada linha do codigo fonte

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

			// Trata o caso de achar uma virgula
			for (i = 0; i < vetor_palavras.size(); i++)
			{
				if (vetor_palavras.at(i).find("0X") != std::string::npos)
				{
					vetor_palavras.at(i) = this->hexad(vetor_palavras.at(i));
				}

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
                }
                else if (this->apoio.find(":") != std::string::npos) {
                    this->saida = this->apoio;
                }
                else {
					if (this->apoio != "") 
					{
						this->saida = this->apoio + "\n";
					}
					else this->saida = this->apoio;
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

/*******************************************************************
 * Funcao que transforma hexadecimal em decimal *
 ******************************************************************/

string Assembler::hexad(string word) {
	int numero, contaconta, j, i = 0;
	int eax, saida = 0;

	word = word.substr(2, word.size());


	for (j = word.size() - 1; j != -1; j--)
	{
		if (word.at(j) == 'A') {
			contaconta = 0;
			eax = 10;
			while (contaconta < i)
			{
				eax = eax * 16;
				contaconta++;
			}
			saida += eax;
		}
		else if (word.at(j) == 'B') {
			contaconta = 0;
			eax = 11;
			while (contaconta < i)
			{
				eax = eax * 16;
				contaconta++;
			}
			saida += eax;
		}
		else if (word.at(j) == 'C') {
			contaconta = 0;
			eax = 12;
			while (contaconta < i)
			{
				eax = eax * 16;
				contaconta++;
			}
			saida += eax;
		}
		else if (word.at(j) == 'D') {
			contaconta = 0;
			eax = 13;
			while (contaconta < i)
			{
				eax = eax * 16;
				contaconta++;
			}
			saida += eax;
		}
		else if (word.at(j) == 'E') {
			contaconta = 0;
			eax = 14;
			while (contaconta < i)
			{
				eax = eax * 16;
				contaconta++;
			}
			saida += eax;
		}
		else if (word.at(j) == 'F') {
			contaconta = 0;
			eax = 15;
			while (contaconta < i)
			{
				eax = eax * 16;
				contaconta++;
			}
			saida += eax;
		}
		else {

			contaconta = 0;
			eax = word.at(j) - 48;

			while (contaconta < i)
			{
				eax = eax * 16;
				contaconta++;
			}
			saida += eax;

		}
		i++;
	}


	return to_string(saida);
}

/***********************************************
 * Funcao que separa as palavras de cada linha *
 **********************************************/
void Assembler::passagemUm(){
    int i = 0, posicaotabela = 0;

	this->linha_coluna_contador = 0;

    ifstream codigoPreProcessado(this->file_pre);     // Abre o arquivo que contem o codigo pre-processado
    ofstream codigoObjeto(this->file_obj);     // Abre o arquivo texto que contera o codigo objeto

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
        cout << "Erro semantico: Secao TEXT faltante" << endl;
    }

	cout << saida << endl;

    codigoObjeto << saida;
    codigoObjeto.close();          // Fecha o arquivo com o codigo fonte
}


/*************************************************
 * Funcao que checa se a palavra eh um mneumonico *
 ************************************************/
void Assembler::checaMneumonico(int *posicaotabela) {
    int i,j;
	int flagtipo, tamanho;
    for (i = 0; i < this->vetor_palavras.size(); i++) {
		// Reconhecimento dos campos TEXT e DATA
		if (this->vetor_palavras.at(i) == "SECTION") {
            if(this->vetor_palavras.size() > i+1){
                if (this->vetor_palavras.at(i + 1) == "TEXT") {
                    this->text_field_start = this->pc_pre_processado;
                    section_text_present = true;
                }
                else if (this->vetor_palavras.at(i + 1) == "DATA") {
                    this->data_field_start = this->pc_pre_processado;
                }
                else
                {
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Secao invalido" << endl;
                }
            }
            else{
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Secao invalido" << endl;
            }

            i++;    // pula palavra posterior (no caso TEXT ou DATA)
        }
        // Reconhecimento de diretivas e instrucoes
        else if (this->vetor_palavras.at(i) == "CONST") {
			this->linha_coluna_contador++;
			this->opcodes.push_back(this->vetor_palavras.at(i + 1));
			if (this->vetor_palavras.at(i+1) == "0") {
				cout << "Erro semantico linha: "<< pc_pre_processado << "  tentativa de dividir por 0" << endl;
			}
            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado < data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Diretiva "<< this->vetor_palavras.at(i)
                         <<" na secao errada" << endl;
                }
            }
            // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado > text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Diretiva "<< this->vetor_palavras.at(i)
                     <<" na secao errada" << endl;
            }

            i++; // pula palavra posterior
        }
        else if (this->vetor_palavras.at(i) == "SPACE") {
			this->linha_coluna_contador++;

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado < data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Diretiva "<< this->vetor_palavras.at(i)
                         <<" na secao errada " << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado > text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Diretiva "<< this->vetor_palavras.at(i)
                     <<" na secao errada " << endl;
            }

			// Caso de declaracao de um vetor
            if (this->vetor_palavras.size() - 1 > i) {
				this->linha_coluna_contador += stoi(this->vetor_palavras.at(i + 1)) - 1;
                for (j = 0; j < stoi(this->vetor_palavras.at(i + 1)); j++) {
                    this->opcodes.push_back("00");
                }

				i++;    //Pula palavra posterior
            }
            // Caso de declaracao de uma variavel
            else{
                this->opcodes.push_back("00");
            }
        }
        else if (this->vetor_palavras.at(i) == "ADD") {
			this->opcodes.push_back("1");           // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
			trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico:Iinstrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "SUB") {
            this->opcodes.push_back("2");           // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
			trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "MULT") {
            this->opcodes.push_back("3");           // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "DIV") {
            this->opcodes.push_back("4");           // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "JMP") {
            this->opcodes.push_back("5");           // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "JMPN") {
            this->opcodes.push_back("6");           // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "JMPP") {
            this->opcodes.push_back("7");           // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "JMPZ") {
            this->opcodes.push_back("8");           // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "COPY") {
            this->opcodes.push_back("9"); 
			this->linha_coluna_contador++;
			// Coloca o opcode da instrucao no vetor
            trataErros(&i, 2);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-2)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-2)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "LOAD") {
            this->opcodes.push_back("10");          // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "STORE") {
            this->opcodes.push_back("11");          // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "INPUT") {
            this->opcodes.push_back("12");          // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "OUTPUT") {
            this->opcodes.push_back("13");          // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;
            trataErros(&i, 1);          // Chama a funcao de tratamento de erros

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i-1)
                     <<" na secao errada" << endl;
            }
        }
        else if (this->vetor_palavras.at(i) == "STOP") {
            this->opcodes.push_back("14");          // Coloca o opcode da instrucao no vetor
			this->linha_coluna_contador++;

            // Erro de diretiva na secao errada
            // Secao DATA posterior a secao TEXT
            if(data_field_start > text_field_start){
                if(pc_pre_processado > data_field_start){
                    cout << "Linha " << pc_pre_processado << "; Erro semantico: Instrucao "<< this->vetor_palavras.at(i)
                         <<" na secao errada" << endl;
                }
            }
                // Secao DATA anterior a secao TEXT
            else if(pc_pre_processado < text_field_start){
                cout << "Linha " << pc_pre_processado << "; Erro semantico: instrucao "<< this->vetor_palavras.at(i)
                     <<" na secao errada" << endl;
            }
        }
        // if nao gerais
        // label definida
        else if (this->vetor_palavras.at(i).find(":") != std::string::npos)
        {
			//this->linha_coluna_contador--;
			if (this->vetor_palavras.at(i+1) == "SPACE") {
				//valor a ser passado para procuraPendencias que sinaliza que o rotulo eh tipo SPACE
				flagtipo = 0;
				tamanho = 1;
				//se tem space vetorizado conserta o valor de tamanho
				if (this->vetor_palavras.size() == 3) {
					tamanho = stoi(this->vetor_palavras.at(i + 2)) - 1;
				
				}
			}

			else if (this->vetor_palavras.at(i + 1) == "CONST") {
				//valor a ser passado para procuraPendencias que sinaliza que o rotulo eh tipo CONST
				flagtipo = 1;
				tamanho = 1;
			}
			else {
				//valor a ser passado para procuraPendencias que sinaliza que o rotulo eh tipo Rotulo para JMP
				flagtipo = 2;
				tamanho = 1;
			}
            this->apoio = this->vetor_palavras.at(i).substr(0, this->vetor_palavras.at(i).find(":"));
            tabela_de_simbolos->procuraPendencias(this->apoio, this->linha_coluna_contador, &this->opcodes, flagtipo, tamanho, pc_pre_processado);
            posicaotabela--;
		}
        //label nao definida
        else {
            cout << "Linha " << pc_pre_processado << "; Erro lexico: instrucao ou diretiva "<< this->vetor_palavras.at(i)
                 <<" invalida" << endl;
            i++;
        }
        posicaotabela++;
		
    }
}


/*************************************************************
 * Funcao trata dos erros presentes do codigo pre-processado *
 ************************************************************/
void Assembler::trataErros(int *posicao_da_palavra, int n_operandos) {
	string aux;
	int auxint;

    // Erros na quantidade de operandos
    if(n_operandos == 1)
    {
        if(this->vetor_palavras.size() - *posicao_da_palavra != 2){
            cout << "Linha " << pc_pre_processado << "; Erro sintatico: Quantidade de operandos invalida para a operacao" << endl;
        }
        else{
            // Logica para lidar com vetores no unico operando
			if (this->vetor_palavras.at(*posicao_da_palavra + 1).find("+") != std::string::npos) {
				aux = this->vetor_palavras.at(*posicao_da_palavra + 1).substr(0, this->vetor_palavras.at(*posicao_da_palavra + 1).find("+"));
				aux = this->tabela_de_simbolos->procuraElemento(aux, this->linha_coluna_contador, this->vetor_palavras.at(*posicao_da_palavra), pc_pre_processado);
				auxint = stoi(aux) + stoi(vetor_palavras.at(*posicao_da_palavra + 1).substr(this->vetor_palavras.at(*posicao_da_palavra + 1).find("+")+1, this->vetor_palavras.at(*posicao_da_palavra + 1).size()-1));
				aux = to_string(auxint);
				this->opcodes.push_back(aux);
			}
			else {
				aux = this->tabela_de_simbolos->procuraElemento(this->vetor_palavras.at(*posicao_da_palavra + 1), this->linha_coluna_contador, this->vetor_palavras.at(*posicao_da_palavra), pc_pre_processado);
				this->opcodes.push_back(aux);
            }
			this->linha_coluna_contador++;
            *posicao_da_palavra+=1;    // pula palavra posterior
        }
    }
    else if(n_operandos == 2){
        if(this->vetor_palavras.size() <= *posicao_da_palavra+2){
            cout << "Linha " << pc_pre_processado << "; Erro sintatico: Quantidade de operandos invalida para a operacao" << endl;
        }
        else{
            // Logica para lidar com vetores no primeiro operando
            if (this->vetor_palavras.at(*posicao_da_palavra + 1).find("+") != std::string::npos) {
				aux = this->vetor_palavras.at(*posicao_da_palavra + 1).substr(0, this->vetor_palavras.at(*posicao_da_palavra + 1).find("+"));
				aux = this->tabela_de_simbolos->procuraElemento(aux, this->linha_coluna_contador, this->vetor_palavras.at(*posicao_da_palavra), pc_pre_processado);
				auxint = stoi(aux) + stoi(vetor_palavras.at(*posicao_da_palavra + 1).substr(this->vetor_palavras.at(*posicao_da_palavra + 1).find("+") + 1, this->vetor_palavras.at(*posicao_da_palavra + 1).size() - 1));
				aux = to_string(auxint);
				this->opcodes.push_back(aux);
			}
			else {
				aux = this->tabela_de_simbolos->procuraElemento(this->vetor_palavras.at(*posicao_da_palavra + 1), this->linha_coluna_contador, this->vetor_palavras.at(*posicao_da_palavra), pc_pre_processado);
				this->opcodes.push_back(aux);
			}

			this->linha_coluna_contador++;

            // Logica para lidar com vetores no segundo operando
            if (this->vetor_palavras.at(*posicao_da_palavra + 2).find("+") != std::string::npos) {
				aux = this->vetor_palavras.at(*posicao_da_palavra + 2).substr(0, this->vetor_palavras.at(*posicao_da_palavra + 2).find("+"));
				aux = this->tabela_de_simbolos->procuraElemento(aux, this->linha_coluna_contador, this->vetor_palavras.at(*posicao_da_palavra), pc_pre_processado);
				auxint = stoi(aux) + stoi(vetor_palavras.at(*posicao_da_palavra + 2).substr(this->vetor_palavras.at(*posicao_da_palavra + 2).find("+") + 1, this->vetor_palavras.at(*posicao_da_palavra + 2).size() - 1));
				aux = to_string(auxint);
				this->opcodes.push_back(aux);
			}
			else {
				aux = this->tabela_de_simbolos->procuraElemento(this->vetor_palavras.at(*posicao_da_palavra + 2), this->linha_coluna_contador, this->vetor_palavras.at(*posicao_da_palavra), pc_pre_processado);
				this->opcodes.push_back(aux);
			}

			this->linha_coluna_contador++;
            
			*posicao_da_palavra+= 1;    // pula palavra posterior
            *posicao_da_palavra+= 1;    // pula palavra posterior

        }
    }

}