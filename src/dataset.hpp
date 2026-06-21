#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include <map>
#include "new_graph.hpp"

using namespace std;

// A assinatura aqui PRECISA ser idêntica à do datast.cpp
void catch_coment_and_value(string pathCSV, graph &g, map<string, node*> &dicionario, int &curr_index);

#endif