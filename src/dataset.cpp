#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "dataset.hpp"
#include "tokenizer.hpp"
#include <set> 

using namespace std;

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

    while (getline(arquivo, linha))
    {
        if (linha.empty())
            continue;
        if (qtdToTest == 1000)
            break;

        size_t lastAspas = linha.find_last_of('"');

        if (lastAspas != string::npos)
        {
            string frase = linha.substr(1, lastAspas - 1);
            string sentiment = linha.substr(lastAspas + 2);

            node *no_comentario = new node();
            no_comentario->data = frase;
            no_comentario->type = TEXT;
            no_comentario->index = curr_index++;

            if (no_comentario->index >= g.nodes_catalog.size())
            {
                g.resize_graph(max((int)g.nodes_catalog.size() * 2, no_comentario->index + 1));
            }
            g.nodes_catalog[no_comentario->index] = no_comentario;

            node *no_sentiment_atual = (sentiment == "positive") ? g.nodes_catalog[0] : g.nodes_catalog[1];

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

                    if (new_word->index >= g.nodes_catalog.size())
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

string classify_review(string frase, graph &g, map<string, node *> &dicionario)
{
    vector<string> word_list = word_catch(frase);

    float score_positivo = 0.0f;
    float score_negativo = 0.0f;
    
    set<string> stopwords = {"and", "the", "a", "an", "is", "it", "to", "of", "this", "was", "in", "that", "i", "for", "with", "as", "on", "but", "are", "be"};

    cout << "\n[DEBUG] Analisando a frase: " << frase << endl;

    for (string word : word_list)
    {
        // 1. Ignora se for Stopword
        if (stopwords.count(word)) {
            cout << "  [PULADA] '" << word << "' (Stopword ignorada)" << endl;
            continue;
        }

        // 2. Ignora se não existir no grafo
        if (!dicionario.count(word)) {
            cout << "  [INEDITA] '" << word << "' (Nao existe no grafo)" << endl;
            continue;
        }

        int word_index = dicionario[word]->index;
        ll_node *atual = g.adj_list[word_index];

        float peso_pos = 0.0f;
        float peso_neg = 0.0f;

        while (atual != nullptr)
        {
            if (atual->data->index == 0) peso_pos = atual->weight;
            else if (atual->data->index == 1) peso_neg = atual->weight;
            
            atual = atual->prox;
        }
        
        // Em vez de somar o peso bruto, somamos a DIFERENÇA. 
        if (peso_pos > peso_neg) {
            score_positivo += (peso_pos - peso_neg);
        } else if (peso_neg > peso_pos) {
            score_negativo += (peso_neg - peso_pos);
        }

        cout << "  -> '" << word << "' | POS: " << peso_pos << " | NEG: " << peso_neg << endl;
    }

    cout << ">>> PONTUACAO FINAL -> POSITIVA: " << score_positivo << " | NEGATIVA: " << score_negativo << endl;

    if (score_positivo > score_negativo) return "positive";
    else if (score_negativo > score_positivo) return "negative";
    else return "neutral";
}