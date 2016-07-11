#include <bits/stdc++.h>


using namespace std;

struct Individuo {
    string agenda;
    string codigoVoo;
    string codigoAviao;
    string origem;
    string destino;
    string chegada;
    string partida;
};

struct Aviao {
    string tipo;
    string codigo;
    string aeroportoAtual;
    vector<string> vooRealizados;
};

struct Voo {
    string origem;
    string horaPartida;
    string destino;
    string tipoAviao;
    string codigoVoo;
    string horaChegada;

};

vector<Aviao> avioesDisponiveis;
vector<Aviao> avioesOperando;
map<int, vector<Voo> > agenda;
vector<Individuo> individuos;
vector<Individuo> aux;

void iniciaParametros() {
    int numeroGeracoes;
    int tamanhoPopulacao;
    int taxaElitismo;
    int taxaMutacao;
    int cruzamento;

}

void lerArquivo();
void populacaoInicial();
double calculaFitness(int indiceIndividuo);
void selecao();
void Cruzamento(int indiceIndividuo);
void Mutacao(int indiceIndividuo);
void elitismo();
void correcaoIndividuo();
void lerArquivos();
void imprimiIndividuos();
void imprimiIndividuo(Individuo ind);
bool checkAirplanes();
void montaAgenda();
void imprimeAgenda();
bool ordenaVoo(Voo a, Voo b);
void preencheAgenda();

int main() {
    lerArquivos(); //chamada da funcao para iniciar os parametros
    checkAirplanes();
    montaAgenda();
    //imprimeAgenda();

    //imprimiIndividuos();
    //cout << individuos.size() << endl;
}

/*funcao que armazena nos vetores globais os valores de VOOS*/
void lerArquivos() {
    string line;
    //string ag, cA,cV,p, c,o, d;
    ifstream myfile("VOOS1.txt");
    Individuo teste;

    stringstream buffer;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            buffer.clear();
            buffer.str(line);
            buffer >> teste.agenda;
            buffer >> teste.codigoVoo;
            buffer >> teste.codigoAviao;
            buffer >> teste.origem;
            buffer >> teste.partida;
            buffer >> teste.destino;
            buffer >> teste.chegada;
            individuos.push_back(teste);
            Voo vooAux;
            vooAux.codigoVoo = teste.codigoVoo;
            vooAux.destino = teste.destino;
            vooAux.horaChegada = teste.chegada;
            vooAux.horaPartida = teste.partida;
            vooAux.origem = teste.origem;
            vooAux.tipoAviao = teste.codigoAviao;

            //montar o cronograma

            for (int j = 1; j <= teste.agenda.size(); j++) {
                if (teste.agenda[j] != '0') {
                    // cout << teste.agenda[j];
                    agenda[teste.agenda[j] - '0'].push_back(vooAux);
                }

            }
            // ordena por ordem de partida

            cout << endl;


        }
        myfile.close();
    } else cout << "Unable to open file";


    for (int i = 1; i <= 7; i++) {
        vector<Voo> aux;
        aux = agenda[i];

        sort(aux.begin(), aux.end(), ordenaVoo);


        agenda[i].clear();
        agenda[i] = aux;
        /*
        for (int j = 0; j < aux.size(); j++) {
            cout << agenda[i][j].codigoVoo << " ";
            cout << agenda[i][j].destino << " ";
            cout << agenda[i][j].horaChegada << "-> ";
            cout << agenda[i][j].horaPartida << " ";
            cout << agenda[i][j].tipoAviao << endl;
        }
         * */


    }

}

//ordena os voos por hora de chegada

bool ordenaVoo(Voo a, Voo b) {

    int tam = 0;

    int horaA = 0;
    int d = 1;
    for (int i = a.horaPartida.size() - 1; i > 0; i--) {
        if (a.horaPartida[i] >= '0' && a.horaPartida[i] <= '9') {
            //cout << a.horaPartida[i];
            horaA += (a.horaPartida[i] - '0') * d;
            d *= 10;
        }
    }
    // horaB
    int horaB = 0;
    d = 1;
    for (int i = b.horaPartida.size() - 1; i > 0; i--) {
        if (b.horaPartida[i] >= '0' && b.horaPartida[i] <= '9') {
            //cout << a.horaPartida[i];
            horaB += (b.horaPartida[i] - '0') * d;
            d *= 10;

        }

    }


    return (a.horaPartida < b.horaPartida);

}

void imprimeAgenda() {
    // imprime a agenda semana
    Voo aux;
    for (int i = 1; i <= 7; i++) {
        // imprimeVoo = agenda[i];
        cout << "dia da semanaXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << i << " \n";
        cout << agenda[i].size() << " \n";

        for (int k = 0; k < agenda[i].size(); ++k) {
            cout << agenda[i][k].codigoVoo << " ";
            cout << agenda[i][k].destino << " ";
            cout << agenda[i][k].horaChegada << " ";
            cout << agenda[i][k].horaPartida << " ";
            cout << agenda[i][k].tipoAviao << endl;
        }


        //imprimeVoo.clear();
    }

}

void imprimiIndividuo(Individuo ind) {
    cout << ind.agenda << endl;
    cout << ind.codigoAviao << endl;
    cout << ind.codigoVoo << endl;
    cout << ind.partida << endl;
    cout << ind.chegada << endl;


}

void imprimiIndividuos() {
    for (int i = 0; i < individuos.size(); i++) {
        imprimiIndividuo(individuos[i]);
    }
}

bool checkAirplanes() {
    map<string, int> frota;
    ifstream myfile("AVIOES.txt");
    string line, codigo, tipo;
    stringstream buffer;
    Aviao aux;
    /*recebe todos os codigos de avioes marca os valores com 1*/
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            buffer.clear();
            buffer.str(line);
            buffer >> codigo;
            buffer >> tipo;
            aux.codigo = codigo;
            aux.tipo = tipo;
            aux.aeroportoAtual = "NONE";
            avioesDisponiveis.push_back(aux);
        }




        myfile.close();
    } else cout << "Unable to open file";
    /*imprime os avioes da companhia area
    for (int i = 0; i < avioesDisponiveis.size(); i++) {
        cout << avioesDisponiveis[i].codigo << " ";
        cout << avioesDisponiveis[i].tipo << " ";
        cout << avioesDisponiveis[i].aeroportoAtual << " ";
        cout << avioesDisponiveis[i].vooRealizados.size() << "\n";
    }
    cout << "numero de avioes disponiveis " << avioesDisponiveis.size() << endl;
     */
    return true;


}
// função que simula a criaçao de uma agenda semana de voo

void montaAgenda() {
    //
    for (int dia = 1; dia <= 7; dia++) {
        vector<Voo> vooDia;
        vooDia = agenda[dia];
        for (int vook = 0; vook < vooDia.size(); vook++) {
            int randNumber = rand() % avioesDisponiveis.size();
            cout <<"aviao sorteado " << randNumber << endl;
            //verifica se os tipos dos avioes sao iguais
            if(avioesDisponiveis[randNumber].tipo == agenda[dia][vook].tipoAviao){
                cout << "ok" ;
            }else{
                cout <<"X" << avioesDisponiveis[randNumber].tipo << "X X" << agenda[dia][vook].tipoAviao <<"X \n";
                cout << "not ok";
            }
            cout << endl;

        }
    }
}










