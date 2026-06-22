#include <iostream>
#include <cmath>
#include <limits>
#include "graph.hpp"
#include "dataset.hpp"
#include <map>
#include <vector>

using namespace std;

static bool distances_equal(float a, float b)
{
    if (std::isinf(a) && std::isinf(b))
        return true;
    return std::fabs(a - b) < 1e-6f;
}

static string predict_sentiment(graph &g, node *comment, node *positivo, node *negativo)
{
    float dist_pos = g.djikstra(comment, positivo);
    float dist_neg = g.djikstra(comment, negativo);

    if (distances_equal(dist_pos, dist_neg))
        return "neutral";

    if (dist_pos < dist_neg)
        return "positive";

    return "negative";
}

int main()
{
    graph g(10000);

    node *positivo = new node();
    positivo->data = "POSITIVO";
    positivo->type = SENTIMENT;
    positivo->index = 0;
    g.nodes_catalog[0] = positivo;

    node *negativo = new node();
    negativo->data = "NEGATIVO";
    negativo->type = SENTIMENT;
    negativo->index = 1;
    g.nodes_catalog[1] = negativo;

    node *neutro = new node();
    neutro->data = "NEUTRO";
    neutro->type = SENTIMENT;
    neutro->index = 2;
    g.nodes_catalog[2] = neutro;

    int curr_index = 3;
    map<string, node *> dicionario;
    vector<test_sample> test_samples;

    cout << "Povoando o grafo com as 10.000 linhas processadas do IMDB..." << endl;
    catch_coment_and_value("data/IMDB_processed.csv", g, dicionario, curr_index, test_samples);
    cout << "Grafo preenchido com sucesso!" << endl;

    cout << "\nClassificando " << test_samples.size() << " comentarios de teste (20%)..." << endl;

    int correct = 0;
    int neutral_predictions = 0;
    int positive_predictions = 0;
    int negative_predictions = 0;

    for (const test_sample &sample : test_samples)
    {
        string predicted = predict_sentiment(g, sample.comment, positivo, negativo);

        if (predicted == "neutral")
            neutral_predictions++;
        else if (predicted == "positive")
            positive_predictions++;
        else
            negative_predictions++;

        if (predicted == sample.true_sentiment)
            correct++;
    }

    float accuracy = test_samples.empty() ? 0.0f
                                          : (100.0f * correct) / static_cast<float>(test_samples.size());

    cout << "\n--- Resultados ---" << endl;
    cout << "Acuracia: " << accuracy << "% (" << correct << "/" << test_samples.size() << ")" << endl;
    cout << "Predicoes positive: " << positive_predictions << endl;
    cout << "Predicoes negative: " << negative_predictions << endl;
    cout << "Predicoes neutral (empate): " << neutral_predictions << endl;

    cout << "\n--- Classificando comentarios novos ---" << endl;
    string frase_1 = "This cinematographic artwork was absolutely beautiful and amazing!";
    string frase_2 = "Worst experience ever. Extremely boring, terrible direction and bad acting.";

    cout << "Frase 1: " << frase_1 << endl;
    cout << "Resultado: " << classify_review(frase_1, g, dicionario) << endl;

    cout << "\nFrase 2: " << frase_2 << endl;
    cout << "Resultado: " << classify_review(frase_2, g, dicionario) << endl;

    return 0;
}
