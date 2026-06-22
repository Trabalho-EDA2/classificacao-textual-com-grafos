#include "tokenizer.hpp"
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>




using namespace std;

vector<string> word_catch(string phrase)
{
    vector<string> words;
    string word = "";
    
        for (char c : phrase)
        {
            if (isalnum(c))
            {
                word += c;
            }
            else if(!word.empty()){
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                
                words.push_back(word);

                word = "";
            }
        }

        if (!word.empty())
        {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            words.push_back(word);
        }
    

    return words;
}
