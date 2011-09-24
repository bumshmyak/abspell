

#include <corrections_generator.h>
#include <trie_based_dictionary.h>
#include <dummy_phrase_corrections_generator.h>
#include <levenshtein_corrections_generator.h>

#include <using_std.h>

int main(int argc, char** argv) {

  if (argc < 2) {
    cerr << "USAGE: " << argv[0] << " <dict file> " << endl;
    return 1;
  }

  TTrieDictionary dict;
  dict.LoadFromFile(argv[1]);
  dict.SetMaxDistance(2);

  LevenshteinWordCandidatesGenerator word_corrector(dict);
  BestWordPhraseCandidatesGenerator phrase_corrector(word_corrector);

  while (!cin.eof()) {
    string line;
    getline(cin, line);
    
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
