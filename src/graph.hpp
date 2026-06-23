#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <algorithm>
#include <cstdio>
#include <cstddef>
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
    float weight;
} ll_node;

struct bfs_visit
{
    node *data;
    int depth;
};

struct bfs_queue_item
{
    node *data;
    int depth;
};

// Fila circular implementada sem std::queue.
class bfs_queue
{
    bfs_queue_item *items;
    size_t capacity;
    size_t head;
    size_t count;

    void grow();

public:
    explicit bfs_queue(size_t initial_capacity = 16);
    ~bfs_queue();

    bfs_queue(const bfs_queue &) = delete;
    bfs_queue &operator=(const bfs_queue &) = delete;

    bool empty() const;
    void enqueue(node *data, int depth);
    bool dequeue(bfs_queue_item &item);
};

// Se fizessemos em só uma struct o nó do grafo teria que guardar referências da
// lista de adjacência, isso fica estranho

class graph
{
    int n;
    int curr_index;
    
    public:
    vector<ll_node *> adj_list;
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
    vector<bfs_visit> bounded_bfs(
        node *start,
        int max_depth,
        const vector<node_type> &allowed_types_by_depth = {}) const;

private:
    // Tem de adicionar o '&' no ll_head para bater com o seu .cpp!
    void add_ll_node(ll_node *&ll_head, node *new_vertex);
    void remove_from_ll(ll_node *&ll_head, int target_index);
};

#endif
