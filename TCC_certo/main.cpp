#include <bits/stdc++.h>

using namespace std;

struct Individuo {
    string diasDaSemana;
    string codigoVoo;
    string codigoAviao;
    string origem;
    string destino;
    string duracao;
    string partida;
};

struct Aviao {
    string tipo;
    string codigo;
    string aeroportoAtual;
    vector<string> vooRealizados;
    int vooInuteis;
};

struct Voo {
    string origem;
    string horaPartida;
    int dia_partida;
    int dia_chegada;
    string horaChegada;
    string destino;
    string tipoAviao;
    string codigoVoo;
    string duracao;

};

struct aviao_essencial {
    string codigo;
    string tipo;
};

//no vetor de avioes vao ficar salvo os avioes em valores absolutas nao podendo deletar nenhum aviao
map<string, vector<Aviao> > avioes;
//o map de avioes disponiveis é apenas um artificio auxiliar para conseguir manipular a atribuiçoes de voos
map<string, vector<Aviao> > avioesDisponiveis;

map<string, vector<aviao_essencial> > marca_volta;
map<int, vector<Voo> > agenda;
vector<Individuo> individuos;
int proximo_dia;
string hora = "0000";

void iniciaParametros() {
    int numeroGeracoes;
    int tamanhoPopulacao;
    int taxaElitismo;
    int taxaMutacao;
    int cruzamento;
}

void insereVooEmAviao(string TipoAviao, string codigoAviao, string codigoVoo, string AeroportoDoVoo);
void apagaAvioesDisponiveis(int posicao);
void imprimiString(string a);
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
void mostra_avioes_operacao();
bool checkAirplanes();
void montaAgenda();
void insere_de_volta_disponiveis(string tempo_inicio, string tempo_fim);
void imprimeAgenda();
bool ordenaVoo(Voo a, Voo b);
void preencheAgenda();
bool comparaString(string a, string b);
string trataString(string a);
void imprimiFrotaAtribuida();
string calcula_Hora_chegada(string d, string h, int dia);

int main() {
    lerArquivos(); //chamada da funcao para iniciar os parametros
    checkAirplanes();
    imprimiFrotaAtribuida();
    /*

    aviao_essencial aux;
    aux.codigo ="blabla";
    aux.tipo= "E190";
    marca_volta["92047"].push_back(aux);
    marca_volta["92047"].push_back(aux);
    insere_de_volta_disponiveis("92045", "2100");
     */
    //imprimiFrotaAtribuida();

    montaAgenda();
    //imprimiFrotaAtribuida();
    //imprimeAgenda();
    //apagaAvioesDisponiveis(0);


    //imprimiIndividuos();
    //cout << individuos.size() << endl;
}

string trataString(string a) {
    string b = "";
    for (int i = 1; i < a.size(); i += 2) {
        b.push_back(a[i]);
    }
    return b;

}

/*funcao que armazena nos vetores globais os valores de VOOS*/
void lerArquivos() {
    string line;


    //string ag, cA,cV,p, c,o, d;
    ifstream myfile("VOO2.txt");
    Individuo teste;
    int contador = 0;
    stringstream buffer;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            //cout << "tamanho da linha " << line.size() << endl;
            buffer << line;
            buffer >> teste.diasDaSemana;
            buffer >> teste.codigoVoo;
            buffer >> teste.codigoAviao;
            buffer >> teste.origem;
            buffer >> teste.partida;
            buffer >> teste.destino;
            buffer >> teste.duracao;
            contador++;
            buffer.clear();
            individuos.push_back(teste);
            Voo vooAux;
            vooAux.codigoVoo = teste.codigoVoo;
            vooAux.destino = teste.destino;
            vooAux.duracao = teste.duracao;
            vooAux.horaPartida = teste.partida;
            vooAux.origem = teste.origem;
            vooAux.tipoAviao = teste.codigoAviao;


            for (int j = 0; j < teste.diasDaSemana.size(); j++) {
                if (teste.diasDaSemana[j] != '0') {
                    vooAux.horaChegada = calcula_Hora_chegada(vooAux.duracao, vooAux.horaPartida, j + 1);
                    vooAux.dia_partida = j + 1;
                    vooAux.dia_chegada = proximo_dia;
                    agenda[j + 1].push_back(vooAux);
                }

            }


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
            cout << agenda[i][j].destino << " -> Duracao ";
            cout << agenda[i][j].duracao << "-> Partida";
            cout << agenda[i][j].horaPartida << "dia ";
            cout << agenda[i][j].dia_partida << "-> chegada";
            cout << agenda[i][j].horaChegada << " dia";
            cout << agenda[i][j].dia_chegada << " "    ;
            cout << agenda[i][j].tipoAviao << endl;
            
        }
         */




    }


}

//ordena os voos por hora de partida

string calcula_Hora_chegada(string d, string h, int dia) {

    int minutostotais = 0;
    proximo_dia = dia;
    string horas = h.substr(0, 2);
    string minutos = h.substr(2, h.size());
    minutostotais += stoi(minutos);
    int horatotais = stoi(horas);
    horatotais *= 60;
    minutostotais += horatotais;

    string horas_duracao = d.substr(0, 2);
    string minutos_duracao = d.substr(2, h.size());
    minutostotais += stoi(minutos_duracao);
    int horatotais_duracao = stoi(horas_duracao);
    horatotais_duracao *= 60;
    minutostotais += horatotais_duracao;

    int hora_chegada = (int) ((minutostotais) / 60);
    if (hora_chegada >= 24) {
        proximo_dia++;
        hora_chegada %= 24;
    }
    int minutos_chegada = minutostotais % 60;
    string hora_certa = to_string((int) hora_chegada);
    if (hora_certa.size() == 1) {
        hora_certa = "0" + hora_certa;
    }
    hora_certa += to_string(minutos_chegada);


    return hora_certa;
}

void mostra_avioes_operacao() {
    cout << "avioes que vao voltar \n";
    map<string, vector<aviao_essencial> >::iterator it;
    for (it = marca_volta.begin(); it != marca_volta.end(); it++) {
        cout << "tempo = " << it->first;
        cout << " cod=";
        vector<aviao_essencial> hue;
        hue = it->second;
        for (int i = 0; i < hue.size(); i++) {
            cout << hue[i].codigo << " tipo" << hue[i].tipo << endl;
        }
    }
}

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
    //Voo aux;
    for (int i = 1; i <= 7; i++) {
        // imprimeVoo = agenda[i];
        cout << "dia da semanaXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << i << " \n";
        cout << agenda[i].size() << " \n";
        for (int k = 0; k < agenda[i].size(); ++k) {
            cout << "Codigo->" << agenda[i][k].codigoVoo << " ";
            cout << "Destino->" << agenda[i][k].destino << " ";
            cout << "Duracao->" << agenda[i][k].duracao << " ";
            cout << "HoraPartida->" << agenda[i][k].horaPartida << " ";
            cout << "TipoAviao->" << agenda[i][k].tipoAviao << endl;
        }
        //imprimeVoo.clear();
    }
}

void imprimiAviao(Aviao a) {
    cout << "aeroportoAtual " << a.aeroportoAtual << endl;
    cout << "codigo do aviao " << a.codigo << endl;
    cout << "voo inuteis" << a.vooInuteis << endl;
    cout << "voos realizados" << endl;
    for (int i = 0; i < a.vooRealizados.size(); i++) {
        cout << a.vooRealizados[i] << " ";
    }
    cout << endl;
}

void imprimiIndividuo(Individuo ind) {
    cout << ind.diasDaSemana << endl;
    cout << ind.codigoAviao << endl;
    cout << ind.codigoVoo << endl;
    cout << ind.partida << endl;
    cout << ind.duracao << endl;
}

void imprimiIndividuos() {
    for (int i = 0; i < individuos.size(); i++) {
        imprimiIndividuo(individuos[i]);
    }
}

bool checkAirplanes() {

    ifstream myfile("AVIOES.txt");
    string line, codigo, tipo;
    stringstream buffer;
    Aviao aux;
    /*recebe todos os codigos de avioes marca os valores com 1*/
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            buffer.clear();
            buffer.str(line);
            trataString(line);
            buffer >> codigo;
            buffer >> tipo;
            aux.codigo = codigo;
            aux.tipo = tipo;
            aux.aeroportoAtual = "NONE";
            aux.vooInuteis = 0;
            //cout <<"Codigo do aviao " << tipo<< endl;
            avioesDisponiveis[tipo].push_back(aux);
            avioes[tipo].push_back(aux);
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

bool comparaString(string a, string b) {
    return true;
}

void imprimiFrotaAtribuida() {
    map<string, vector<Aviao> >::iterator it;
    vector<Aviao> auxAviao;
    for (it = avioesDisponiveis.begin(); it != avioesDisponiveis.end(); it++) {

        auxAviao = it->second;
        int tam = auxAviao.size();
        cout << "tipo ->" << it->first << "tamanho[" <<tam  << "]\n";

    }
}

void imprimiString(string a) {
    for (int i = 0; i < a.size(); i++) {
        cout << "[" << a[i] << "]";

    }
    cout << endl;
}

void insereVooEmAviao(string TipoAviao, string codigoAviao, string codigoVoo, string AeroportoDoVoo) {

    // busca sequencial N*log(N) onde N é o numero de avioes do Tipo que chamou a funcao
    int tam = avioes[TipoAviao].size();
    for (int i = 0; i < tam; i++) {
        if (avioes[TipoAviao][i].codigo == codigoAviao) {
            avioes[TipoAviao][i].vooRealizados.push_back(codigoVoo);
            if (avioes[TipoAviao][i].aeroportoAtual != AeroportoDoVoo) {
                avioes[TipoAviao][i].vooInuteis++;
            }
        }
    }


}

void insere_de_volta_disponiveis(string tempo_inicio, string tempo_fim) {

    cout << "\n\n\n--------comeco da funcaooo-----\n";
    cout << "TEEEMPOO INICIO = " << tempo_inicio << "TEMPO FIM " << tempo_fim << endl;
    cout << "avioes operando \n";
    mostra_avioes_operacao();
    cout << "\n\n";
    cout << "frota antes de verificar \n";
    imprimiFrotaAtribuida();

    int hora_inicio = stoi(tempo_inicio);
    int hora_fim = stoi(tempo_fim);
    int minuto_inicio = stoi(tempo_inicio);
    int minuto_fim = stoi(tempo_fim);
    int dia_atual = stoi(tempo_inicio);
    minuto_inicio = minuto_inicio % 100;
    minuto_fim = minuto_fim % 100;
    hora_inicio = (hora_inicio / 100) % 100;
    hora_fim = ((hora_fim) / 100) % 100;
    dia_atual /= 10000;

    while (true) {

        if ((hora_inicio == hora_fim) && (minuto_inicio == minuto_fim)) {
            break;
        } else {
            string volta = to_string(minuto_inicio);
            if (volta.size() == 1) {
                volta = "0" + volta;
            }
            volta = to_string(hora_inicio) + volta;
            if (volta.size() == 3) {
                volta = "0" + volta;
            }
            volta = to_string(dia_atual) + volta;
            map<string, vector<aviao_essencial> >::iterator it;
            it = marca_volta.find(volta);
            if (it != marca_volta.end()) {

                vector<aviao_essencial> aux;
                aux = it->second;
                for (int i = 0; i < aux.size(); i++) {
                    aviao_essencial aux2;
                    aux2 = aux[i];
                    Aviao Aux3;
                    Aux3.codigo = aux2.codigo;
                    avioesDisponiveis[aux2.tipo].push_back(Aux3);
                }
            }
            minuto_inicio++;
            if (minuto_inicio == 60) {
                hora_inicio++;
                minuto_inicio = 0;
            }

        }
    }
    cout << "frota no fim da funcao \n\n";
    imprimiFrotaAtribuida();
    cout << "-------fim da funcao -----------------\n";
}

void montaAgenda() {
    int contador = 0;

    vector<Aviao> AviaoAux;
    map<string, vector<Aviao> >::iterator it2;


    for (int dia = 1; dia <= 8; dia++) {


        vector<Voo> vooDia; // vetor auxiliar para manipular os dias
        vooDia = agenda[dia]; // cada map de int, vector de voos representa um dia
        vector< Aviao>::iterator it; // iterator auxiliar pra operaçẽos no vector
        //varre todos os voos do dia
        string hora_inicial = "0000";
        string tempo_inicial = to_string(dia);
        tempo_inicial = tempo_inicial + hora_inicial;
        for (int vook = 0; vook < vooDia.size(); vook++) {


            insere_de_volta_disponiveis(tempo_inicial, agenda[dia][vook].horaPartida);
            string tipoAviao = agenda[dia][vook].tipoAviao;
            // funcao random para gerar numeros aleatorios de forma rapida
            std::random_device rd; // only used once to initialise (seed) engine
            std::mt19937 rng(rd()); // random-number engine used (Mersenne-Twister in this case)  
            std::uniform_int_distribution<int> uni(1, avioesDisponiveis[tipoAviao].size());
            auto randNumber = uni(rng);

            // busca os avioes nos avioes disponiveis

            it2 = avioesDisponiveis.find(tipoAviao);
            AviaoAux = it2->second;
            Aviao Av;
            Av = AviaoAux[randNumber - 1];

            //cria variaveis para inserir no vetor de avioes absolutos
            string codigoA, codigoV;
            codigoA = Av.codigo;
            codigoV = agenda[dia][vook].codigoVoo;
            string Aero = agenda[dia][vook].origem;

            insereVooEmAviao(tipoAviao, codigoA, codigoV, Aero);
            string volta = "";
            volta = to_string(agenda[dia][vook].dia_chegada);
            volta += agenda[dia][vook].horaChegada;
            // apaga de avioes disponiveis
            int tam = avioesDisponiveis[tipoAviao].size();
            cout << tipoAviao << endl;
            cout << "posicao sorteada para apagar " << randNumber - 1 <<
                    avioesDisponiveis[tipoAviao][randNumber - 1].codigo << "\n";
            avioesDisponiveis[tipoAviao].erase(avioesDisponiveis[tipoAviao].begin() + (randNumber - 1));
            aviao_essencial X;
            X.codigo = codigoA;
            X.tipo = tipoAviao;
            marca_volta[volta].push_back(X);
            tempo_inicial = to_string(dia) + agenda[dia][vook].horaPartida;


        }
        insere_de_volta_disponiveis(tempo_inicial, "2359");

    }
    cout << "numero de voos realizados " << contador;
}