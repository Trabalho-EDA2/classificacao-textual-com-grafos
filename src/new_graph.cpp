#include <algorithm>
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

typedef enum node_type {TEXT, WORD, SENTIMENT};

typedef struct node { //nó do grafo
    string data;
    node_type type;
    int index;
    // float weight;
} node;

typedef struct ll_node { //nó da lista encadeada
    node* data;
    ll_node* prox;
} ll_node;

// Se fizessemos em só uma struct o nó do grafo teria que guardar referências da 
// lista de adjacência, isso fica estranho


class graph {
    int n;
    vector<ll_node*> adj_list;
    int curr_index; //ainda tem que implementar a lógica de incremento conforme novos nós do grafo são adicionados 

    public:
    graph(int n) {
        //nao precisa de this pq n tem conflito de nomes
        adj_list = vector<ll_node*>(n, nullptr);
        this->n = n;
        curr_index = 0;
    };

    void resize_graph(int new_n){ //dessa forma já aumenta o tamanho preservando os dados
        adj_list.resize(new_n);
        n = new_n;
    }

    bool has_edge(node* node1, node* node2){
        //tem que acessar no vetor e buscar na lista
    }

    private: void add_ll_node(ll_node* ll_head, node *new_node ){
        ll_node *new_ll_node;
        new_ll_node->data = new_node;

        if(ll_head != nullptr){
            new_ll_node->prox = ll_head->prox;
            ll_head->prox = new_ll_node;
        } else {
            ll_node *new_ll_head;
            new_ll_head->data = nullptr;
            new_ll_head->prox = new_ll_node;
            new_ll_node->prox = nullptr;
            ll_head = new_ll_head;
        }
    }

    void add_edge(node* node1, node* node2){//fazer overload para o peso da aresta
        //verificar se node_type é diferente - grafos partidos não podem ter ligação entre elementos do mesmo tipo
        if(!has_edge(node1, node2) && node1->type != node2->type){
            if(max(node1->index, node2->index)+1 >= n){
                resize_graph(max(2*n, node1->index, node2->index));
            }

            add_ll_node(adj_list[node1->index], node2);
            add_ll_node(adj_list[node2->index], node1);
        }
    };


    void remove_edge(int node1, int node2){

    };

    void print_adj_matrix(){

    }
};
