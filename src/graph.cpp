#include "graph.hpp"
#include <algorithm>
#include <cstdio>
#include <vector>
#include <string>
#include <map>

using namespace std;

bfs_queue::bfs_queue(size_t initial_capacity)
{
    capacity = max<size_t>(initial_capacity, 1);
    items = new bfs_queue_item[capacity];
    head = 0;
    count = 0;
}

bfs_queue::~bfs_queue()
{
    delete[] items;
}

bool bfs_queue::empty() const
{
    return count == 0;
}

void bfs_queue::grow()
{
    size_t new_capacity = capacity * 2;
    bfs_queue_item *new_items = new bfs_queue_item[new_capacity];

    for (size_t i = 0; i < count; i++)
    {
        new_items[i] = items[(head + i) % capacity];
    }

    delete[] items;
    items = new_items;
    capacity = new_capacity;
    head = 0;
}

void bfs_queue::enqueue(node *data, int depth)
{
    if (count == capacity)
    {
        grow();
    }

    size_t tail = (head + count) % capacity;
    items[tail] = {data, depth};
    count++;
}

bool bfs_queue::dequeue(bfs_queue_item &item)
{
    if (empty())
    {
        return false;
    }

    item = items[head];
    head = (head + 1) % capacity;
    count--;
    return true;
}

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

// Adiciona uma aresta bidirecional (se os tipos forem diferentes)
void graph::add_edge(node *node1, node *node2)
{
    if (node1 == nullptr || node2 == nullptr || node1->type == node2->type) return;

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
            // 👈 AGORA IMPRIME O NOME DO VIZINHO E O PESO DA ARESTA!
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

vector<bfs_visit> graph::bounded_bfs(
    node *start,
    int max_depth,
    const vector<node_type> &allowed_types_by_depth) const
{
    vector<bfs_visit> visits;

    if (start == nullptr || max_depth < 0 || start->index < 0 ||
        start->index >= static_cast<int>(nodes_catalog.size()))
    {
        return visits;
    }

    if (!allowed_types_by_depth.empty() &&
        (allowed_types_by_depth.size() <= static_cast<size_t>(max_depth) ||
         start->type != allowed_types_by_depth[0]))
    {
        return visits;
    }

    vector<bool> visited(nodes_catalog.size(), false);
    bfs_queue queue;

    visited[start->index] = true;
    queue.enqueue(start, 0);

    bfs_queue_item current;
    while (queue.dequeue(current))
    {
        visits.push_back({current.data, current.depth});

        if (current.depth == max_depth)
        {
            continue;
        }

        int next_depth = current.depth + 1;
        ll_node *neighbor = adj_list[current.data->index];

        while (neighbor != nullptr)
        {
            node *next = neighbor->data;
            bool valid_index = next != nullptr && next->index >= 0 &&
                               next->index < static_cast<int>(visited.size());
            bool valid_type = allowed_types_by_depth.empty() ||
                              next->type == allowed_types_by_depth[next_depth];

            if (valid_index && valid_type && !visited[next->index])
            {
                visited[next->index] = true;
                queue.enqueue(next, next_depth);
            }

            neighbor = neighbor->prox;
        }
    }

    return visits;
}
