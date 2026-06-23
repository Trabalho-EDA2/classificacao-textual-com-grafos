#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include <map>
#include <vector>
#include "graph.hpp"

using namespace std;

struct test_sample
{
    node *comment;
    string true_sentiment;
};

void catch_coment_and_value(string pathCSV, graph &g, map<string, node *> &dicionario,
                            int &curr_index, vector<test_sample> &test_samples);

string classify_review(string frase, graph &g, map<string, node *> &dicionario, int &curr_index);

#endif
