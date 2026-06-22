#include "graph.hpp"
#include "queue.hpp"
#include <algorithm>
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <limits>

using namespace std;

// Construtor da classe
graph::graph(int n)
{
    adj_list = vector<ll_node *>(n, nullptr);
    nodes_catalog = vector<node *>(n, nullptr);
    this->n = n;
    curr_index = 0;
}

// Método para redimensionar o grafo preservando os dados
void graph::resize_graph(int new_n)
{
    adj_list.resize(new_n, nullptr);
    nodes_catalog.resize(new_n, nullptr);
    n = new_n;
}

// Verifica se existe uma aresta entre dois nós
bool graph::has_edge(node *node1, node *node2)
{
    if (node1 == nullptr || node2 == nullptr)
        return false;

    if (node1->index >= n)
        return false;

    ll_node *atual = adj_list[node1->index];

    while (atual != nullptr)
    {
        if (atual->data == node2)
            return true;

        atual = atual->prox;
    }

    return false;
}

int graph::get_size() const
{
    return n;
}

ll_node *graph::get_adj_head(int index) const
{
    if (index < 0 || index >= n)
        return nullptr;
    return adj_list[index];
}

static bool can_connect(node *node1, node *node2)
{
    if (node1->type == node2->type)
        return node1->type == WORD;
    return true;
}

void graph::add_edge(node *node1, node *node2)
{
    if (node1 == nullptr || node2 == nullptr || !can_connect(node1, node2))
        return;
    
    if (max(node1->index, node2->index) + 1 >= n)
    {
        resize_graph(max({2 * n, node1->index + 1, node2->index + 1}));
    }

    add_ll_node(adj_list[node1->index], node2);
    add_ll_node(adj_list[node2->index], node1);
    
}

// Remove uma aresta com base nos índices
void graph::remove_edge(int node1, int node2)
{
    if (node1 >= n || node2 >= n)
        return;

    remove_from_ll(adj_list[node1], node2);
    remove_from_ll(adj_list[node2], node1);
}

// Imprime a lista de adjacência do grafo
void graph::print_adj_list()
{
    for (int i = 0; i < n; i++)
    {
        string nome_dono = (nodes_catalog[i] != nullptr) ? nodes_catalog[i]->data : "VAZIO";

        printf("No [%d](%s) se conecta com:\n", i, nome_dono.c_str());

        ll_node *current = adj_list[i];
        while (current != nullptr)
        {
            printf("  -> | %s | (peso: %.1f)\n", current->data->data.c_str(), current->weight);
            current = current->prox;
        }
        printf("\n");
    }
}

// Método privado para inserir um nó no início da lista encadeada
void graph::add_ll_node(ll_node * &ll_head, node * new_node)
{
    ll_node* atual = ll_head;

    while(atual != nullptr){
        if(atual->data == new_node){
            atual->weight += 1.0f;
            return;
        }
        atual = atual->prox;
    }

    ll_node *new_ll_node = new ll_node();
    new_ll_node->data = new_node;
    new_ll_node->weight = 1.0f;
    new_ll_node->prox = ll_head;
    ll_head = new_ll_node;
}

// Método privado auxiliar para remover um nó específico de uma lista encadeada
void graph::remove_from_ll(ll_node * &ll_head, int target_index)
{
    ll_node *atual = ll_head;
    ll_node *anterior = nullptr;

    while (atual != nullptr && atual->data->index != target_index)
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == nullptr)
        return;

    if (anterior == nullptr)
    {
        ll_head = atual->prox;
    }
    else
    {
        anterior->prox = atual->prox;
    }

    delete atual;
}

static float edge_cost(float weight)
{
    if (weight <= 0.0f)
        return numeric_limits<float>::infinity();
    return 1.0f / weight;
}

float graph::djikstra(node *origem, node *destino) const
{
    if (origem == nullptr || destino == nullptr)
        return numeric_limits<float>::infinity();

    vector<float> dist(n, numeric_limits<float>::infinity());
    vector<bool> in_queue(n, false);

    Queue fila;
    dist[origem->index] = 0.0f;
    fila.enqueue(origem->index);
    in_queue[origem->index] = true;

    while (!fila.is_empty())
    {
        int u = fila.dequeue();
        in_queue[u] = false;

        ll_node *atual = get_adj_head(u);
        while (atual != nullptr)
        {
            int v = atual->data->index;
            float custo = edge_cost(atual->weight);

            if (dist[u] + custo < dist[v])
            {
                dist[v] = dist[u] + custo;
                if (!in_queue[v])
                {
                    fila.enqueue(v);
                    in_queue[v] = true;
                }
            }

            atual = atual->prox;
        }
    }

    return dist[destino->index];
}
