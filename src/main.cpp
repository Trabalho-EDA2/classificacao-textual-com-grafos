#include <iostream>
#include <cstdlib>
#include "new_graph.hpp"
#include "tokenizer.hpp"
#include <map>
#include "dataset.hpp"

using namespace std;

int main(){
   // system("python scripts/preprocess.py");
   graph g(10000);

    node* positivo = new node();
    positivo->data = "POSITIVO";
    positivo->type = SENTIMENT;
    positivo->index = 0;
    g.nodes_catalog[0] = positivo;

    node* negativo = new node();
    negativo->data = "NEGATIVO";
    negativo->type = SENTIMENT;
    negativo->index = 1;
    g.nodes_catalog[1] = negativo;

    node* neutro = new node();
    neutro->data = "NEUTRO";
    neutro->type = SENTIMENT;
    neutro->index = 2;
    g.nodes_catalog[2] = neutro;

    int curr_index = 3;
    map<string, node*> dicionario;

    // teste de inserção e arestas no grafo
    // string texto_teste = "amazing movie i love it";
    // node* no_comentario = new node();
    
    // no_comentario->data = texto_teste;
    // no_comentario->type = TEXT;
    // no_comentario->index = curr_index++;
    // g.nodes_catalog[no_comentario->index] = no_comentario;
    
    // g.add_edge(no_comentario, positivo);

    // vector<string> word_list = word_catch(texto_teste);

    // for(string word: word_list){
    //     if(!dicionario.count(word)){
    //         node* new_word = new node();
    //         new_word->data = word;
    //         new_word->type = WORD;
    //         new_word->index = curr_index++;

    //         g.nodes_catalog[new_word->index] = new_word;

    //         dicionario[word] = new_word;
    //         g.add_edge(no_comentario, new_word);
    //     }else{
    //         node* word_existence = dicionario[word];

    //         g.add_edge(no_comentario, word_existence);
    //     }
    // }
    // g.print_adj_list();

    // o teste finaliza aqui

    cout << "Povoando o grafo com as 10.000 linhas do IMDB..." << endl;

    // 3. CHAMADA REAL DA SUA FUNÇÃO DO DATASET!
    // Toda aquela lógica de criar nós de comentários e palavras vai acontecer aqui dentro.
    catch_coment_and_value("data/IMDB_dataset.csv", g, dicionario, curr_index);

    cout << "Grafo preenchido com sucesso!" << endl;
    
    g.print_adj_list();
    
    return 0;
}