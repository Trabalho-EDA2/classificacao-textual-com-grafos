#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;
using matrix_type = vector<vector<int>>;

class graph {
    matrix_type matrix;
    int n;

    public:
    graph(int n){
        matrix = matrix_type(n, vector<int>(n, 0)); //nao precisa de this pq n tem conflito de nomes
        this->n = n;
    };

    void resize_graph(int new_n){ //dessa forma já aumenta o tamanho preservando os dados
        matrix.resize(new_n);

        for(auto &row : matrix){
            row.resize(new_n, 0);
        }
        n = new_n;
    }

    bool has_edge(int node1, int node2){
        if(max(node1, node2) < n) 
            return matrix[node1][node2];
        return false;
    }

    void add_edge(int node1, int node2){
        int required_size = max(node1, node2) + 1;

        if(required_size > n){
            resize_graph(max(2 * n, required_size));
        }

        matrix[node1][node2] = matrix[node2][node1] = 1;
    };

    void remove_edge(int node1, int node2){
        if(has_edge(node1, node2)) 
            matrix[node1][node2] = matrix[node2][node1] = 0;
    };

    void print_adj_matrix(){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }
};


int main(){

}