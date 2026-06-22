#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <algorithm>
#include <cstdio>
#include <vector>
#include <string>
#include <limits>

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
    float weight;
} ll_node;

// Se fizessemos em só uma struct o nó do grafo teria que guardar referências da
// lista de adjacência, isso fica estranho

class graph
{
    int n;
    int curr_index;
    
    public:
    vector<ll_node *> adj_list;
    vector<node*> nodes_catalog;
    graph(int n);
    void resize_graph(int new_n);
    int get_size() const;
    ll_node *get_adj_head(int index) const;

    bool has_edge(node *node1, node *node2);
    void add_edge(node *node1, node *node2);
    void remove_edge(int node1, int node2);
    void print_adj_list();
    float djikstra(node *origem, node *destino) const;

private:    // Tem de adicionar o '&' no ll_head para bater com o seu .cpp!
    void add_ll_node(ll_node *&ll_head, node *new_vertex);
    void remove_from_ll(ll_node *&ll_head, int target_index);
};

#endif
