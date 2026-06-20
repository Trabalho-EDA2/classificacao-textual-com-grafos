#ifndef NEW_GRAPH
#define NEW_GRAPH
#include <algorithm>
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

enum node_type
{
    TEXT,
    WORD,
    SENTIMENT
};

typedef struct vertex
{ // nó do grafo
    string data;
    node_type type;
    int index;
    // float weight;
} node;

typedef struct ll_node
{ // nó da lista encadeada
    node *data;
    ll_node *prox;
} ll_node;

// Se fizessemos em só uma struct o nó do grafo teria que guardar referências da
// lista de adjacência, isso fica estranho

class graph
{
    int n;
    vector<ll_node *> adj_list;
    int curr_index;
    
    public:
    vector<node*> nodes_catalog;
    graph(int n); // Construtor limpo
    void resize_graph(int new_n);
    void add_vertex(string data, node_type type); 
    void remove_vertex();                         
    node *has_vertex();                           // Trocado vertex* por node*

    // Todas abaixo usando node* e sem o "graph::" na frente:
    bool has_edge(node *node1, node *node2);
    void add_edge(node *node1, node *node2);
    void remove_edge(int node1, int node2);
    void print_adj_list(); // Corrigido o "grpah" para "graph"

private:
    // Tem de adicionar o '&' no ll_head para bater com o seu .cpp!
    void add_ll_node(ll_node *&ll_head, node *new_vertex);
    void remove_from_ll(ll_node *&ll_head, int target_index);
};

#endif