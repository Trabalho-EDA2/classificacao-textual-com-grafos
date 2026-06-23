#include <fstream>
#include <iostream>
#include <map>
#include <set>
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

    bool parse_dataset_line(const string &line, string &text, string &sentiment)
    {
        size_t separator = line.find_last_of(',');
        if (separator == string::npos)
        {
            return false;
        }

        text = line.substr(0, separator);
        sentiment = trim_carriage_return(line.substr(separator + 1));

        if (text.size() >= 2 && text.front() == '"' && text.back() == '"')
        {
            text = text.substr(1, text.size() - 2);
        }
        if (sentiment.size() >= 2 && sentiment.front() == '"' && sentiment.back() == '"')
        {
            sentiment = sentiment.substr(1, sentiment.size() - 2);
        }

        return !text.empty() && (sentiment == "positive" || sentiment == "negative");
    }
}

void catch_coment_and_value(
    string pathCSV,
    graph &g,
    map<string, node *> &dicionario,
    int &curr_index,
    int max_reviews)
{
    ifstream arquivo(pathCSV);

    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o dataset!\n";
        return;
    }

    string linha;
    getline(arquivo, linha);

    int qtdToTest = 0;

    while (getline(arquivo, linha))
    {
        if (linha.empty())
            continue;
        if (qtdToTest == max_reviews)
            break;

        string frase;
        string sentiment;
        if (parse_dataset_line(linha, frase, sentiment))
        {
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

            node *no_comentario = new node();
            no_comentario->data = frase;
            no_comentario->type = TEXT;
            no_comentario->index = curr_index++;

            if (no_comentario->index >= static_cast<int>(g.nodes_catalog.size()))
            {
                g.resize_graph(max(static_cast<int>(g.nodes_catalog.size()) * 2,
                                   no_comentario->index + 1));
            }
            g.nodes_catalog[no_comentario->index] = no_comentario;
            g.add_edge(no_comentario, no_sentiment_atual);

            vector<string> word_list = word_catch(frase);

            for (const string &word : word_list)
            {
                node *word_node = nullptr;
                auto dictionary_entry = dicionario.find(word);

                if (dictionary_entry == dicionario.end())
                {
                    node *new_word = new node();
                    new_word->data = word;
                    new_word->type = WORD;
                    new_word->index = curr_index++;

                    if (new_word->index >= static_cast<int>(g.nodes_catalog.size()))
                    {
                        g.resize_graph(max(static_cast<int>(g.nodes_catalog.size()) * 2,
                                           new_word->index + 1));
                    }
                    g.nodes_catalog[new_word->index] = new_word;
                    dicionario[word] = new_word;
                    word_node = new_word;
                }
                else
                {
                    word_node = dictionary_entry->second;
                }

                // A classificacao usa o caminho WORD -> TEXT -> SENTIMENT.
                // Nao existe mais uma aresta direta entre WORD e SENTIMENT.
                g.add_edge(no_comentario, word_node);
            }
        }
        qtdToTest++;
    }
}

string classify_review(
    string frase,
    graph &g,
    map<string, node *> &dicionario,
    bool verbose)
{
    vector<string> word_list = word_catch(frase);
    set<string> stopwords = {
        "and", "the", "a", "an", "is", "it", "to", "of", "this", "was",
        "in", "that", "i", "for", "with", "as", "on", "but", "are", "be"};

    set<string> query_words;
    map<int, int> matches_by_review;
    const vector<node_type> classification_path = {WORD, TEXT, SENTIMENT};

    for (const string &word : word_list)
    {
        if (!stopwords.count(word))
        {
            query_words.insert(word);
        }
    }

    if (verbose)
    {
        cout << "\n[DEBUG] Analisando a frase: " << frase << endl;
    }

    for (const string &word : query_words)
    {
        auto dictionary_entry = dicionario.find(word);
        if (dictionary_entry == dicionario.end())
        {
            if (verbose)
            {
                cout << "  [INEDITA] '" << word << "' (Nao existe no grafo)" << endl;
            }
            continue;
        }

        vector<bfs_visit> visits = g.bounded_bfs(
            dictionary_entry->second, 2, classification_path);

        int reviews_found = 0;
        for (const bfs_visit &visit : visits)
        {
            if (visit.depth == 1 && visit.data->type == TEXT)
            {
                matches_by_review[visit.data->index]++;
                reviews_found++;
            }
        }

        if (verbose)
        {
            cout << "  -> '" << word << "' alcancou " << reviews_found
                 << " reviews por BFS" << endl;
        }
    }

    float score_positivo = 0.0f;
    float score_negativo = 0.0f;

    for (const auto &[review_index, matched_words] : matches_by_review)
    {
        node *review = g.nodes_catalog[review_index];
        node *sentiment = nullptr;
        int distinct_words_in_review = 0;
        ll_node *neighbor = g.adj_list[review_index];

        while (neighbor != nullptr)
        {
            if (neighbor->data->type == WORD)
            {
                distinct_words_in_review++;
            }
            else if (neighbor->data->type == SENTIMENT)
            {
                sentiment = neighbor->data;
            }

            neighbor = neighbor->prox;
        }

        if (review == nullptr || sentiment == nullptr || distinct_words_in_review == 0)
        {
            continue;
        }

        float review_score = static_cast<float>(matched_words) /
                             static_cast<float>(distinct_words_in_review);

        if (sentiment->index == 0)
        {
            score_positivo += review_score;
        }
        else if (sentiment->index == 1)
        {
            score_negativo += review_score;
        }
    }

    if (verbose)
    {
        cout << ">>> REVIEWS CANDIDATAS: " << matches_by_review.size() << endl;
        cout << ">>> PONTUACAO FINAL -> POSITIVA: " << score_positivo
             << " | NEGATIVA: " << score_negativo << endl;
    }

    if (score_positivo > score_negativo)
        return "positive";
    if (score_negativo > score_positivo)
        return "negative";

    // Em caso de empate, usa como desempate a classe mais frequente no treino.
    int reviews_positivas = 0;
    int reviews_negativas = 0;

    for (ll_node *neighbor = g.adj_list[0]; neighbor != nullptr; neighbor = neighbor->prox)
    {
        if (neighbor->data->type == TEXT)
            reviews_positivas++;
    }
    for (ll_node *neighbor = g.adj_list[1]; neighbor != nullptr; neighbor = neighbor->prox)
    {
        if (neighbor->data->type == TEXT)
            reviews_negativas++;
    }

    return reviews_positivas >= reviews_negativas ? "positive" : "negative";
}

void test_accuracy(
    string pathCSV,
    graph &g,
    map<string, node *> &dicionario,
    int linhas_treino,
    int linhas_teste)
{
    ifstream arquivo(pathCSV);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o dataset para teste!\n";
        return;
    }

    string linha;
    getline(arquivo, linha); // Pula o cabecalho.

    int linhas_lidas = 0;
    while (linhas_lidas < linhas_treino && getline(arquivo, linha))
    {
        if (!linha.empty())
        {
            linhas_lidas++;
        }
    }

    cout << "\n==================================================" << endl;
    cout << "INICIANDO TESTE DE ACURACIA COM " << linhas_teste
         << " FRASES INEDITAS..." << endl;
    cout << "==================================================" << endl;

    int acertos = 0;
    int erros = 0;
    int testes_feitos = 0;

    while (testes_feitos < linhas_teste && getline(arquivo, linha))
    {
        if (linha.empty())
        {
            continue;
        }

        string frase;
        string gabarito;
        if (!parse_dataset_line(linha, frase, gabarito))
        {
            continue;
        }

        string palpite = classify_review(frase, g, dicionario, false);

        if (palpite == gabarito)
        {
            acertos++;
        }
        else
        {
            erros++;
        }
        testes_feitos++;
    }

    float acuracia = testes_feitos > 0
                         ? (static_cast<float>(acertos) / testes_feitos) * 100.0f
                         : 0.0f;

    cout << "\nRESULTADO FINAL DO MODELO:" << endl;
    cout << "Total de testes: " << testes_feitos << endl;
    cout << "Acertos: " << acertos << endl;
    cout << "Erros: " << erros << endl;
    cout << "ACURACIA: " << acuracia << "%" << endl;
    cout << "==================================================\n" << endl;
}
