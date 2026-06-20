#ifndef  NEW_GRAPH
#define NEW_GRAPH
#include <algorithm>
#include <cstdio>
#include <vector>
#include <string>

enum node_type {TEXT, WORD, SENTIMENT};

typedef struct vertex { //nó do grafo
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
    int curr_index;

    public:
    graph(int n) {};
    void resize_graph(int new_n){};
    void add_vertex(string data, node_type type){};
    void remove_vertex(){};
    vertex* has_vertex(){};
    bool has_edge(vertex* node1, vertex* node2){};
    void add_edge(vertex* node1, vertex* node2){};
    void remove_edge(vertex* node1, vertex* node2){};
    void print_adj_list(){};

    private: 
    void add_ll_node(ll_node* ll_head, vertex *new_vertex ){};
};

#endif