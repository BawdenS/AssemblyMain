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
                    this->pc_pre_processado++;
                }
                else if (this->apoio.find(":") != std::string::npos) {
                    this->saida = this->apoio;
                }
                else {
                    this->saida = this->apoio + "\n";
                    this->pc_pre_processado++;
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
    /*
    this->word[0] = "NULL";
    this->word[1] = "NULL";
    this->word[2] = "NULL";
    this->word[3] = "NULL";
    */

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

    }

    codigoPreProcessado.close();   // Fecha o arquivo com o codigo pre-processado

    this->saida.clear();
    // Coloca os opcodes no codigo objeto
    for (i = 0; i < opcodes.size(); i++) {

        saida += opcodes.at(i) + " ";
    }

    cout << saida;
    codigoObjeto << saida;
    codigoObjeto.close();          // Fecha o arquivo com o codigo fonte

}


/*************************************************
 * Funcao que checa se a palavra eh um mneumonico *
 ************************************************/
void Assembler::checaMneumonico(int *posicaotabela) {
    /*
	if(word[0] == "SECTION"){
        if(word[1] == "TEXT"){
            //cout << "        EH DIRETIVA SECTION TEXT!" << endl;
            this->text_field_start = this->pc_pre_processado;
            cout << "Campo Text comeca na linha " << this->text_field_start << endl;
        }
        else if(word[1] == "DATA"){
            //cout << "        EH DIRETIVA SECTION DATA!" << endl;
            this->data_field_start = this->pc_pre_processado;
            cout << "Campo Data comeca na linha " << this->data_field_start << endl;
        }
    }
    else if(word[0] == "SPACE"){
        this->line_opcode = 00;
        opcodes.emplace_back(this->line_opcode);

        if(word[1] != "NULL"){
            //cout << "        EH DIRETIVA SPACE E VALOR: " << word[1] << " OPCODE = " << this->line_opcode << this->line_opcode << endl;
        }
        else{
            //cout << "        EH DIRETIVA SPACE E VALOR: 1 OPCODE = " << this->line_opcode << this->line_opcode << endl;
        }
    }
    else if(word[0] == "CONST"){
        //cout << "        EH DIRETIVA CONST  E VALOR: " << word[1] << endl;
    }
    else if(word[0] == "EQU"){
        //cout << "        EH DIRETIVA EQU  E VALOR: " << word[1] << endl;
    }
    else if(word[0] == "IF"){
        //cout << "        EH DIRETIVA IF!" << endl;
    }
    else if(word[0] == "ADD"){
        this->line_opcode = 1;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO ADD!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "SUB"){
        this->line_opcode = 2;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO SUB!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "MULT"){
        this->line_opcode = 3;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO MULT!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "DIV"){
        this->line_opcode = 4;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO DIV!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "JMP"){
        this->line_opcode = 5;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO JMP!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "JMPN"){
        this->line_opcode = 6;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO JMPN!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "JMPP"){
        this->line_opcode = 7;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO JMPP!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "JMPZ"){
        this->line_opcode = 8;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO JMPZ!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "COPY"){
        this->line_opcode = 9;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO COPY!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "LOAD"){
        this->line_opcode = 10;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO LOAD!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "STORE"){
        this->line_opcode = 11;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO STORE!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "INPUT"){
        this->line_opcode = 12;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO INPUT!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "OUTPUT"){
        this->line_opcode = 13;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO OUTPUT!     OPCODE = " << this->line_opcode << endl;
    }
    else if(word[0] == "STOP"){
        this->line_opcode = 14;
        opcodes.emplace_back(this->line_opcode);
        //cout << "        EH INSTRUCAO STOP!     OPCODE = " << this->line_opcode << endl;
    }
    else{
        //cout << "        NAO EH MNEUMONICO, EH SIMBOLO! ";
        if(word[1] == "SPACE"){
            this->line_opcode = 00;
            opcodes.emplace_back(this->line_opcode);
            if(word[2] != "NULL"){
                //cout << "        EH DIRETIVA SPACE E VALOR: " << word[1] << " OPCODE = " << this->line_opcode << this->line_opcode << endl;
            }
            else{
                //cout << "        EH DIRETIVA SPACE E VALOR: 1 OPCODE = " << this->line_opcode << this->line_opcode << endl;
            }
        }
        else if(word[1] == "CONST"){
            //cout << "SEGUIDO DE DIRETIVA CONST E VALOR: " << word[2] << endl;
        }
        else if(word[1] == "EQU"){
            //cout << "SEGUIDO DE DIRETIVA EQU  E VALOR: " << word[2] << endl;
        }
        else if(word[1] == "IF"){
            //cout << "SEGUIDO DE DIRETIVA IF!" << endl;
        }
        else if(word[1] == "ADD"){
            this->line_opcode = 1;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO ADD!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "SUB"){
            this->line_opcode = 2;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO SUB!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "MULT"){
            this->line_opcode = 3;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO MULT!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "DIV"){
            this->line_opcode = 4;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO DIV!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "JMP"){
            this->line_opcode = 5;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO JMP!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "JMPN"){
            this->line_opcode = 6;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO JMPN!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "JMPP"){
            this->line_opcode = 7;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO JMPP!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "JMPZ"){
            this->line_opcode = 8;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO JMPZ!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "COPY"){
            this->line_opcode = 9;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO COPY!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "LOAD"){
            this->line_opcode = 10;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO LOAD!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "STORE"){
            this->line_opcode = 11;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO STORE!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "INPUT"){
            this->line_opcode = 12;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO INPUT!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "OUTPUT"){
            this->line_opcode = 13;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO OUTPUT!     OPCODE = " << this->line_opcode << endl;
        }
        else if(word[1] == "STOP"){
            this->line_opcode = 14;
            opcodes.emplace_back(this->line_opcode);
            //cout << "        EH INSTRUCAO STOP!     OPCODE = " << this->line_opcode << endl;
        }
    }
	*/

    int i,j;
    for (i = 0; i < this->vetor_palavras.size(); i++) {
        //if gerais

        if (this->vetor_palavras.at(i) == "CONST") {

            this->opcodes.push_back(this->vetor_palavras.at(i + 1));

            i++; // pula palavra posterior
        }
        else if (this->vetor_palavras.at(i) == "SPACE") {
            // Caso de declaracao de um vetor
            if (this->vetor_palavras.size() - 1 > i) {
                for (j = 0; j < stoi(this->vetor_palavras.at(i + 1)); j++) {
                    this->opcodes.push_back("00");
                }
            }
            // Caso de declaracao de uma variavel
            else
                this->opcodes.push_back("00");
        }
        // if nao gerais
        // label definida
        else if (this->vetor_palavras.at(i).find(":") != std::string::npos)
        {
            this->apoio = this->vetor_palavras.at(i).substr(0, this->vetor_palavras.at(i).find(":"));
            tabela_de_simbolos->procuraPendencias(this->apoio, *posicaotabela);
            posicaotabela--;
        }
            //todo douradao
            //label nao definida
        else {


        }
        posicaotabela++;
    }
}