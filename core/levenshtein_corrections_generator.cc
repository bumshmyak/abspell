#include <using_std.h>

#include <cmath>

#include "levenshtein_corrections_generator.h"

using std::string;
using std::vector;

void LevenshteinWordCandidatesGenerator::GetCandidates(
    const string& word,
    vector<CorrectionCandidate>& candidates,
    double threshold) const {
  
  vector<string> candidate_words;
  dictionary_.GetNeighbourWords(word, &candidate_words);

  vector<CorrectionCandidate> candidates_unsorted;
  for (size_t index = 0; index < candidate_words.size(); ++index) {
    double levenshtein_distance = get_levenshtein_distance(word, candidate_words[index], 10);
//    double misspell_probability_ = 1.0 - 2.0*levenshtein_distance / (word.size() + candidate_words[index].size()); 
    
    double candidate_frequency_ = dictionary_.GetWordFrequency(candidate_words[index]);
    
    candidates_unsorted.push_back(
        CorrectionCandidate(
          candidate_words[index],
          log(candidate_frequency_ + 1.0) - levenshtein_distance));
  }
  sort(candidates_unsorted.begin(), candidates_unsorted.end());

  if (candidates_unsorted.size()) {
    candidates.push_back(candidates_unsorted.back());
  }
}



