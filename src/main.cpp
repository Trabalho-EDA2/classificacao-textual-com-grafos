#include <cstdlib>
#include "graph.hpp"

using namespace std;

int main(){
    system("python scripts/preprocess.py");

    node* positivo = new node();
    positivo->data = "POSITIVO";
    positivo->type = SENTIMENT;
    positivo->index = 0;
    
    node* negativo = new node();
    negativo->data = "NEGATIVO";
    negativo->type = SENTIMENT;
    negativo->index = 1;

    node* neutro = new node();
    neutro->data = "NEUTRO";
    neutro->type = SENTIMENT;
    neutro->index = 2;

    graph g(3);
    int curr_index = 3;

    map<string, node*> dicionario;

    string texto_teste = "amazing movie i love it";
    node* no_comentario = new node();

    no_comentario->data = texto_teste;
    no_comentario->type = TEXT;
    no_comentario->index = curr_index++;

    g.add_edge(no_comentario, positivo);


    return 0;
}