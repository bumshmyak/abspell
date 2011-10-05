#include <corrections_generator.h>
#include <trie_based_dictionary.h>
#include <hash_based_dictionary.h>
#include <ngram_phrase_corrections_generator.h>
#include <levenshtein_corrections_generator.h>
#include <boost/lexical_cast.hpp>

#include <using_std.h>

int main(int argc, char** argv) {
  if (argc < 4) {
    cerr << "USAGE: " << argv[1] << " <dict file> ";
    cerr  << argv[2] << " <bigram file> ";
    cerr << argv[3] << " <coefficent>" << endl;
    return 1;
  }
  
   // TTrieDictionary dict;
  THashDictionary dict;
  dict.LoadFromFile(argv[1]);
  //dict.SetMaxDistance(1);

  const double WordPenalty = 0.1;

  // LevenshteinWordCandidatesGenerator word_corrector(dict);
  SimpleLevenshteinWordCandidatesGenerator word_corrector(dict);
  
  const double Coeff = boost::lexical_cast<double>(argv[3]);
  
  NgramPhraseCorrector phrase_corrector(word_corrector, WordPenalty, Coeff);
  phrase_corrector.PrepareBigramDict(argv[2]);

  while (!cin.eof()) {
    string line;
    getline(cin, line);
    if (!line.length()) {
      continue;
    }

    vector<CorrectionCandidate> corrections;
    phrase_corrector.GetCandidates(line, corrections);

    cout << line;
    for (size_t index = 0; index < corrections.size(); ++index) {
      cout << "\t";
      cout << corrections[index].weight_;
      cout << ":";
      cout << corrections[index].text_;
    }
    cout << endl;
  }

  return 0;
}
