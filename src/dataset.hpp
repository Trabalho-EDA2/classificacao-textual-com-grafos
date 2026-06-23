#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include <map>
#include "new_graph.hpp"

using namespace std;

void catch_coment_and_value(string pathCSV, graph &g, map<string, node*> &dicionario, int &curr_index);
string classify_review(string frase, graph &g, map<string, node*> &dicionario);

#endif