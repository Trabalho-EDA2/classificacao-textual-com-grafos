#include "tokenizer.hpp"
#include <sstream>
#include <vector>
#include <string>


using namespace std;

vector<string> word_catch(string phrase){
    stringstream ss(phrase);
    string word;
    vector<string> words;

    while(ss >> word){
        erase_if(word, [](char c){
            return ispunct(c);
        });

        words.push_back(word);
    }

    return words;
}
