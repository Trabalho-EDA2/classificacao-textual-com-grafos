#include "new_graph.hpp"

using namespace std;

class graph {
    int n; //mudar nome para graph_size
    vector<ll_node*> adj_list;
    int curr_index; //ainda tem que implementar a lógica de incremento conforme novos nós do grafo são adicionados 

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

    void add_vertex(string data, node_type type){
        vertex* new_vertex;
        new_vertex->data = data;
        new_vertex->type = type;
        new_vertex->index = curr_index;
        if(curr_index >= n) resize_graph(2*n);
        curr_index++;

        ll_node *new_ll_head;
        new_ll_head->data = new_vertex;
        new_ll_head->prox = nullptr;
        adj_list[curr_index] = new_ll_head;
    }

    void remove_vertex(){

    }

    vertex* has_vertex(){

    }

    bool has_edge(vertex* node1, vertex* node2){
        //tem que acessar no vetor e buscar na lista
        ll_node* current = adj_list[node1->index];

        while(current != nullptr){
            if(current->data == node2) return true;
            current = current->prox;
        }

        return false;
    }

    void add_ll_node(ll_node* ll_head, vertex *new_vertex ){
        if(ll_head != nullptr){
            ll_node *new_ll_node;
            new_ll_node->data = new_vertex;
            new_ll_node->prox = ll_head->prox;
            ll_head->prox = new_ll_node;
        }
    }

    void add_edge(vertex* node1, vertex* node2){//fazer overload para o peso da aresta
        //verificar se node_type é diferente - grafos partidos não podem ter ligação entre elementos do mesmo tipo
        if(!has_edge(node1, node2) && node1->type != node2->type){
            if(max(node1->index, node2->index)+1 >= n){
                resize_graph(max({2*n, node1->index, node2->index}));
            }

            add_ll_node(adj_list[node1->index], node2);
            add_ll_node(adj_list[node2->index], node1);
        }
    };

    void remove_edge(vertex* node1, vertex* node2){
        if(has_edge(node1, node2)){

        }

    };

    void print_adj_list(){
        for(auto &e : adj_list){
            ll_node* current = e;
            while(current != nullptr){
                printf("%s ", current->data->data);
                current = current->prox;
            }
            printf("\n");
        }
    }
};

int main(){
    graph *my_firs_graph = new graph(10);


    return 0;
}