/*!
 *  trie search test appl
 *  loads dictionary formated ass: freq \t word \t doc_freq
 *
 *  reads words from stdin and searches neighbous in trie
 *  
 */

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <trie.h>

using std::vector;
using std::string;
using std::istream;
using std::ifstream;
using std::getline;

using std::back_inserter;
using std::ostream_iterator;

using std::copy;
using std::sort;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

void load_dictionary(TTrie<string>* trie_ptr, istream& is) {
    size_t line_counter = 0;
    while(!is.eof()) {
        string line;
        getline(is, line);
        vector<string> fields;
        boost::split(fields, line, boost::is_any_of("\t"));
    
        if (fields.size() != 3) {
            cerr << "skipping line: " << line_counter << endl;
            continue;
        }
           
        trie_ptr->add(fields[1], fields[1]);

        ++line_counter;
    }
}

int main(int argc, char** argv) {
   
    if (argc < 2) {
        cerr << "USAGE: argv[0] <dict_file>" << endl;;
        return 1;
    }

    TTrie<string> trie;
    ifstream dict(argv[1]);
    load_dictionary(&trie, dict);
    
    while(!cin.eof()) {
        string word;
        cin >> word;

        vector<string> variants;
        trie.find_nearby(word, back_inserter(variants), 2);
    
        cout << "for: [" << word << "] variants: " << variants.size() << " ";
        copy(variants.begin(), variants.end(), ostream_iterator<string>(cout, " "));
        cout << endl;
    }

    return 0;
}
