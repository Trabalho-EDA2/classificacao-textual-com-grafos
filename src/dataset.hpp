#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include <map>
#include "graph.hpp"

using namespace std;

void catch_coment_and_value(
    string pathCSV,
    graph &g,
    map<string, node*> &dicionario,
    int &curr_index,
    int max_reviews);
string classify_review(
    string frase,
    graph &g,
    map<string, node*> &dicionario,
    bool verbose = true);
void test_accuracy(
    string pathCSV,
    graph &g,
    map<string, node *> &dicionario,
    int linhas_treino,
    int linhas_teste);

#endif
