#include <corrections_generator.h>
#include <trie_based_dictionary.h>
#include <hash_based_dictionary.h>
#include <ngram_phrase_corrections_generator.h>
#include <levenshtein_corrections_generator.h>

#include <using_std.h>

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << "USAGE: " << argv[1] << " <dict file> " << endl;
    cerr << "USAGE: " << argv[2] << " <bigram file> " << endl;
    return 1;
  }
  
   // TTrieDictionary dict;
  THashDictionary dict;
  dict.LoadFromFile(argv[1]);
  //dict.SetMaxDistance(1);

  const double WordPenalty = 0.1;

  // LevenshteinWordCandidatesGenerator word_corrector(dict);
  SimpleLevenshteinWordCandidatesGenerator word_corrector(dict);  
  NgramPhraseCorrector phrase_corrector(word_corrector, WordPenalty);
  phrase_corrector.PrepareBigramDict(argv[2]);

  while (!cin.eof()) {
    string line;
    getline(cin, line);
 
    vector<CorrectionCandidate> corrections;
    phrase_corrector.GetCandidates(line, corrections);

    cout << line << endl;
    for (size_t index = 0; index < corrections.size(); ++index) {
      cout << "\t";
      cout << corrections[index].weight_;
      cout << ":";
      cout << corrections[index].text_;
      cout << endl;
    }
    cout << endl;
  }

  return 0;
}
