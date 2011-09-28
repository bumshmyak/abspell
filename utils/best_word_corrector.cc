#include <corrections_generator.h>
#include <trie_based_dictionary.h>
#include <hash_based_dictionary.h>
#include <dummy_phrase_corrections_generator.h>
#include <levenshtein_corrections_generator.h>

#include <boost/shared_ptr.hpp>

#include <using_std.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "USAGE: " << argv[0] << " <dict file> " << endl;
    return 1;
  }
  
  boost::shared_ptr<ISuggestingDictionary> suggesting_dict_ptr;
  boost::shared_ptr<IDictionary> dict_ptr;
  boost::shared_ptr<IWordCandidatesGenerator> word_corrector_ptr;

  if (argc >= 3 && string(argv[2]) == "trie" ) {
    suggesting_dict_ptr.reset(new TTrieDictionary());
    word_corrector_ptr.reset(new LevenshteinWordCandidatesGenerator(*suggesting_dict_ptr));
    reinterpret_cast<TTrieDictionary*>(suggesting_dict_ptr.get())->SetMaxDistance(1);
    suggesting_dict_ptr->LoadFromFile(argv[1]);
  } else {
    dict_ptr.reset(new THashDictionary());
    word_corrector_ptr.reset(new SimpleLevenshteinWordCandidatesGenerator(*dict_ptr));
    dict_ptr->LoadFromFile(argv[1]);
  }

  BestWordPhraseCandidatesGenerator phrase_corrector(*word_corrector_ptr);

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
