#include "tokenizer.hpp"
#include <sstream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

vector<string> word_catch(string phrase)
{
    stringstream ss(phrase);
    string word;
    vector<string> words;

    while (ss >> word)
    {
        string cleaned_word = "";

        
        for (char c : word)
        {
            if (!ispunct(c))
            {
                cleaned_word += static_cast<char>(tolower(static_cast<unsigned char>(c)));
            }
        }

        if (!cleaned_word.empty())
        {
            words.push_back(cleaned_word);
        }
    }

    return words;
}
