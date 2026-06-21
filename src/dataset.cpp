#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "dataset.hpp"
#include "tokenizer.hpp"

using namespace std;

namespace
{
    string trim_carriage_return(string value)
    {
        if (!value.empty() && value.back() == '\r')
        {
            value.pop_back();
        }

        return value;
    }
}

void catch_coment_and_value(string pathCSV, graph &g, map<string, node *> &dicionario, int &curr_index)
{
    ifstream arquivo(pathCSV);

    if (!arquivo.is_open())
    {
        cerr << "Error ao abrir o dataset!\n.";
        return;
    }

    string linha;

    getline(arquivo, linha);

    int qtdToTest = 0;
    const int maxReviews = 10000;

    while (getline(arquivo, linha))
    {
        if (linha.empty())
            continue;
        if (qtdToTest == maxReviews)
            break;

        size_t separator = linha.find_last_of(',');

        if (separator != string::npos)
        {
            string frase = linha.substr(0, separator);
            string sentiment = trim_carriage_return(linha.substr(separator + 1));

            if (frase.empty())
            {
                qtdToTest++;
                continue;
            }

            node *no_comentario = new node();
            no_comentario->data = frase;
            no_comentario->type = TEXT;
            no_comentario->index = curr_index++;

       
            if (no_comentario->index >= static_cast<int>(g.nodes_catalog.size()))
            {
                g.resize_graph(max((int)g.nodes_catalog.size() * 2, no_comentario->index + 1));
            }
            g.nodes_catalog[no_comentario->index] = no_comentario;

            node *no_sentiment_atual = nullptr;
            if (sentiment == "positive")
            {
                no_sentiment_atual = g.nodes_catalog[0];
            }
            else if (sentiment == "negative")
            {
                no_sentiment_atual = g.nodes_catalog[1];
            }
            else
            {
                qtdToTest++;
                continue;
            }

            g.add_edge(no_comentario, no_sentiment_atual);

            vector<string> word_list = word_catch(frase);

            for (string word : word_list)
            {
                node *word_node = nullptr;

                if (!dicionario.count(word))
                {
                    node *new_word = new node();
                    new_word->data = word;
                    new_word->type = WORD;
                    new_word->index = curr_index++;

                    
                    if (new_word->index >= static_cast<int>(g.nodes_catalog.size()))
                    {
                        g.resize_graph(max((int)g.nodes_catalog.size() * 2, new_word->index + 1));
                    }
                    g.nodes_catalog[new_word->index] = new_word;
                    dicionario[word] = new_word;

                    word_node = new_word;
                }
                else
                {
                    node *word_existence = dicionario[word];
                    word_node = word_existence;
                }

                g.add_edge(no_comentario, word_node);
                g.add_edge(word_node, no_sentiment_atual);
            }
        }
        qtdToTest++;
    }
}
