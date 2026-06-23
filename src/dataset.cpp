#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <limits>
#include "dataset.hpp"
#include "tokenizer.hpp"

using namespace std;

namespace
{
    string trim_carriage_return(string value)
    {
        if (!value.empty() && value.back() == '\r')
            value.pop_back();
        return value;
    }

    node *get_or_create_word(graph &g, map<string, node *> &dicionario, int &curr_index, const string &word)
    {
        if (dicionario.count(word))
            return dicionario[word];

        node *new_word = new node();
        new_word->data = word;
        new_word->type = WORD;
        new_word->index = curr_index++;

        if (new_word->index >= static_cast<int>(g.nodes_catalog.size()))
            g.resize_graph(max((int)g.nodes_catalog.size() * 2, new_word->index + 1));

        g.nodes_catalog[new_word->index] = new_word;
        dicionario[word] = new_word;

        return new_word;
    }

    void add_cooccurrence_edges(graph &g, const vector<node *> &unique_words)
    {
        for (size_t i = 0; i < unique_words.size(); i++)
            for (size_t j = i + 1; j < unique_words.size(); j++)
                g.add_edge(unique_words[i], unique_words[j]);
    }
}

void catch_coment_and_value(string pathCSV, graph &g, map<string, node *> &dicionario,
                            int &curr_index, vector<test_sample> &test_samples)
{
    ifstream arquivo(pathCSV);

    if (!arquivo.is_open())
    {
        cerr << "Error ao abrir o dataset!\n.";
        return;
    }

    string linha;
    getline(arquivo, linha);

    int review_count = 0;
    const int maxReviews = 10000;
    const int train_limit = static_cast<int>(maxReviews * 0.8);

    while (getline(arquivo, linha))
    {
        if (linha.empty())
            continue;
        if (review_count == maxReviews)
            break;

        size_t separator = linha.find_last_of(',');

        if (separator == string::npos)
        {
            review_count++;
            continue;
        }

        string frase = linha.substr(0, separator);
        string sentiment = trim_carriage_return(linha.substr(separator + 1));

        if (frase.empty())
        {
            review_count++;
            continue;
        }

        if (sentiment != "positive" && sentiment != "negative")
        {
            review_count++;
            continue;
        }

        bool is_training = review_count < train_limit;

        node *no_comentario = new node();
        no_comentario->data = frase;
        no_comentario->type = TEXT;
        no_comentario->index = curr_index++;

        if (no_comentario->index >= static_cast<int>(g.nodes_catalog.size()))
            g.resize_graph(max((int)g.nodes_catalog.size() * 2, no_comentario->index + 1));

        g.nodes_catalog[no_comentario->index] = no_comentario;

        if (is_training)
        {
            node *no_sentiment_atual = (sentiment == "positive") ? g.nodes_catalog[0] : g.nodes_catalog[1];
            g.add_edge(no_comentario, no_sentiment_atual);
        }
        else
        {
            test_samples.push_back({no_comentario, sentiment});
        }

        vector<string> word_list = word_catch(frase);
        set<string> seen_words;
        vector<node *> unique_word_nodes;

        for (const string &word : word_list)
        {
            node *word_node = get_or_create_word(g, dicionario, curr_index, word);
            g.add_edge(no_comentario, word_node);

            if (!seen_words.count(word))
            {
                seen_words.insert(word);
                unique_word_nodes.push_back(word_node);
            }
        }

        add_cooccurrence_edges(g, unique_word_nodes);

        review_count++;
    }
}

string classify_review(string frase, graph &g, map<string, node *> &dicionario, int &curr_index)
{
    vector<string> word_list = word_catch(frase);

    node *novo_comentario = new node();
    novo_comentario->data = frase;
    novo_comentario->type = TEXT;
    novo_comentario->index = curr_index++;

    if (novo_comentario->index >= static_cast<int>(g.nodes_catalog.size()))
        g.resize_graph(max((int)g.nodes_catalog.size() * 2, novo_comentario->index + 1));

    g.nodes_catalog[novo_comentario->index] = novo_comentario;

    for (const string &word : word_list)
    {
        if (!dicionario.count(word))
            continue;
        g.add_edge(novo_comentario, dicionario[word]);
    }

    node *positivo = g.nodes_catalog[0];
    node *negativo  = g.nodes_catalog[1];

    float dist_pos = g.djikstra(novo_comentario, positivo);
    float dist_neg = g.djikstra(novo_comentario, negativo);

    if (std::isinf(dist_pos) && std::isinf(dist_neg))
        return "neutral";

    if (dist_pos < dist_neg) return "positive";
    if (dist_neg < dist_pos) return "negative";
    return "neutral";
}
