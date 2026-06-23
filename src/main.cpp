#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <fstream>
#include "graph.hpp"
#include "tokenizer.hpp"
#include <map>
#include <string>
#include "dataset.hpp"

using namespace std;

namespace
{
    string to_lowercase(string text)
    {
        transform(text.begin(), text.end(), text.begin(), [](unsigned char character)
        {
            return static_cast<char>(tolower(character));
        });
        return text;
    }

    bool preprocess_keyboard_input(const string &input, string &processed)
    {
        const string input_path = "data/.interactive_input.txt";
        const string output_path = "data/.interactive_processed.txt";

        ofstream input_file(input_path, ios::binary);
        if (!input_file.is_open())
        {
            return false;
        }
        input_file << input;
        input_file.close();

        remove(output_path.c_str());
        int status = system(
            "py -3 scripts/preprocess.py "
            "--text-file data/.interactive_input.txt "
            "--output-text-file data/.interactive_processed.txt");

        ifstream output_file(output_path, ios::binary);
        bool success = status == 0 && output_file.is_open();

        if (success)
        {
            getline(output_file, processed, '\0');
        }

        output_file.close();
        remove(input_path.c_str());
        remove(output_path.c_str());
        return success;
    }
}

int main(){
   // system("python scripts/preprocess.py");
   graph g(4000);
    const string dataset_path = "data/IMDB_processed.csv";
    const int linhas_treino = 3000;
    const int linhas_teste = 1000;

    node* positivo = new node();
    positivo->data = "POSITIVO";
    positivo->type = SENTIMENT;
    positivo->index = 0;
    g.nodes_catalog[0] = positivo;

    node* negativo = new node();
    negativo->data = "NEGATIVO";
    negativo->type = SENTIMENT;
    negativo->index = 1;
    g.nodes_catalog[1] = negativo;

    int curr_index = 2;
    map<string, node*> dicionario;

    // teste de inserção e arestas no grafo
    // string texto_teste = "amazing movie i love it";
    // node* no_comentario = new node();
    
    // no_comentario->data = texto_teste;
    // no_comentario->type = TEXT;
    // no_comentario->index = curr_index++;
    // g.nodes_catalog[no_comentario->index] = no_comentario;
    
    // g.add_edge(no_comentario, positivo);

    // vector<string> word_list = word_catch(texto_teste);

    // for(string word: word_list){
    //     if(!dicionario.count(word)){
    //         node* new_word = new node();
    //         new_word->data = word;
    //         new_word->type = WORD;
    //         new_word->index = curr_index++;

    //         g.nodes_catalog[new_word->index] = new_word;

    //         dicionario[word] = new_word;
    //         g.add_edge(no_comentario, new_word);
    //     }else{
    //         node* word_existence = dicionario[word];

    //         g.add_edge(no_comentario, word_existence);
    //     }
    // }
    // g.print_adj_list();

    // o teste finaliza aqui

    cout << "Povoando o grafo com as linhas processadas do IMDB..." << endl;

    // 3. CHAMADA REAL DA SUA FUNÇÃO DO DATASET!
    // Toda aquela lógica de criar nós de comentários e palavras vai acontecer aqui dentro.
    catch_coment_and_value(
        dataset_path, g, dicionario, curr_index, linhas_treino);

    cout << "Grafo preenchido com sucesso!" << endl;
    
    // g.print_adj_list();

    string frase_teste_1 = "This cinematographic artwork was absolutely beautiful and amazing!";
    string frase_teste_2 = "Worst experience ever. Extremely boring, terrible direction and bad acting.";

    cout << "\n--- Testando o Classificador do Grafo ---" << endl;
    
    cout << "Frase 1: " << frase_teste_1 << endl;
    cout << "Palpite do Grafo: " << classify_review(frase_teste_1, g, dicionario) << endl;
    
    cout << "\nFrase 2: " << frase_teste_2 << endl;
    cout << "Palpite do Grafo: " << classify_review(frase_teste_2, g, dicionario) << endl;

    test_accuracy(
        dataset_path, g, dicionario, linhas_treino, linhas_teste);

    cout << "--- CLASSIFICADOR INTERATIVO ---" << endl;
    cout << "Digite uma review em ingles para classificar." << endl;
    cout << "Use 'sair' ou uma linha vazia para encerrar.\n" << endl;

    string entrada;
    while (true)
    {
        cout << "> ";

        if (!getline(cin, entrada) || entrada.empty())
        {
            break;
        }

        entrada = to_lowercase(entrada);
        if (entrada == "sair")
        {
            break;
        }

        string entrada_processada;
        if (!preprocess_keyboard_input(entrada, entrada_processada))
        {
            cerr << "Nao foi possivel preprocessar a entrada com SpaCy.\n" << endl;
            continue;
        }
        if (entrada_processada.empty())
        {
            cout << "A entrada nao possui palavras validas apos o preprocessamento.\n" << endl;
            continue;
        }

        string resultado = classify_review(
            entrada_processada, g, dicionario, false);
        cout << "Classificacao: " << resultado << "\n" << endl;
    }
    
    return 0;
}
