#include <algorithm>
#include <cstdio>
#include <vector>
#include <string>
#include <map>

using namespace std;

typedef enum node_type
{
    TEXT,
    WORD,
    SENTIMENT
};

typedef struct node
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
    int curr_index; // ainda tem que implementar a lógica de incremento conforme novos nós do grafo são adicionados

public:
    graph(int n)
    {
        // nao precisa de this pq n tem conflito de nomes
        adj_list = vector<ll_node *>(n, nullptr);
        this->n = n;
        curr_index = 0;
    };

    void resize_graph(int new_n)
    { // dessa forma já aumenta o tamanho preservando os dados
        adj_list.resize(new_n);
        n = new_n;
    }

    bool has_edge(node *node1, node *node2)
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
    
    void add_edge(node *node1, node *node2)
    { // fazer overload para o peso da aresta
        // verificar se node_type é diferente - grafos partidos não podem ter ligação entre elementos do mesmo tipo
        if (!has_edge(node1, node2) && node1->type != node2->type)
        {
            if (max(node1->index, node2->index) + 1 >= n)
            {
                resize_graph(max({2 * n, node1->index + 1, node2->index + 1}));
            }
    
            add_ll_node(adj_list[node1->index], node2);
            add_ll_node(adj_list[node2->index], node1);
        }
    };
    
    void remove_edge(int node1, int node2)
    {
        // verifica se os índices fazem sentido no tamanho atual do vetor
        if (node1 >= n || node2 >= n)
            return;
    
        // Remove o node2 da lista do node1
        remove_from_ll(adj_list[node1], node2);
    
        // Remove o node1 da lista do node2
        remove_from_ll(adj_list[node2], node1);
    }
    
    void print_adj_matrix()
    {
    }

private:
    void add_ll_node(ll_node *&ll_head, node *new_node)
    {
        ll_node *new_ll_node = new ll_node();
        new_ll_node->data = new_node;

        // ll_head possui o endereço para caixa A
        //  Adição da caixa B, aponta para caixa A
        new_ll_node->prox = ll_head;

        // atualizo o endereço do head para caixa B
        ll_head = new_ll_node;
    }

    
    // Função auxiliar privada para remover um nó específico de uma lista
    void remove_from_ll(ll_node *&ll_head, int target_index)
    {
        ll_node *atual = ll_head;
        ll_node *anterior = nullptr;

        // Varre a lista procurando o nó com o índice alvo
        while (atual != nullptr && atual->data->index != target_index)
        {
            anterior = atual;    // O anterior assume o lugar do atual
            atual = atual->prox; // O atual dá um passo para frente
        }

        // Se atual for nullptr, significa que a aresta não existia nessa lista
        if (atual == nullptr)
            return;

        // CASO 1: O nó a ser removido é o primeiro da lista
        if (anterior == nullptr)
        {
            ll_head = atual->prox; // A cabeça passa a ser o segundo nó
        }
        // CASO 2: O nó está no meio ou no fim
        else
        {
            anterior->prox = atual->prox; // O anterior pula o atual e aponta para o próximo
        }

        // Deleta a caixinha da memória para não ter vazamento!
        delete atual;
    }
    
};
