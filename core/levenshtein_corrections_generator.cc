#include <using_std.h>

#include <cmath>

#include "levenshtein_corrections_generator.h"

using std::string;
using std::vector;

void LevenshteinWordCandidatesGenerator::GetCandidates(
    const string& word,
    vector<CorrectionCandidate>& candidates,
    double threshold) const {
 
  // dont correct chort words 
  if (word.size() < 8) {
    candidates.push_back(CorrectionCandidate(word, 1.0));
    return;
  }

  vector<string> candidate_words;
  dictionary_.GetNeighbourWords(word, &candidate_words);

  vector<CorrectionCandidate> candidates_unsorted;
  for (size_t index = 0; index < candidate_words.size(); ++index) {
    double levenshtein_distance =
        get_levenshtein_distance(word, candidate_words[index], 10);
    //double misspell_probability_ = 
    //    0.15 - levenshtein_distance / max(word.size(), candidate_words[index].size()); 
    
    double candidate_frequency_ =
        dictionary_.GetWordFrequency(candidate_words[index]);
    
    candidates_unsorted.push_back(
        CorrectionCandidate(
          candidate_words[index],
          log(candidate_frequency_ + 1.0) - 5.0 * levenshtein_distance));
  }
  sort(candidates_unsorted.begin(), candidates_unsorted.end());

  if (candidates_unsorted.size()) {
    candidates.push_back(candidates_unsorted.back());
  }
}

void SimpleLevenshteinWordCandidatesGenerator::GetCandidates(
    const string& word,
    vector<CorrectionCandidate>& candidates,
    double threshold) const {

  // !!! Hack, must be replaced with normal probability
  // based on real distance
  const double CANDIDATE_PROBABILITY = 0.1;
  vector<CorrectionCandidate> precandidates;

  precandidates.push_back(CorrectionCandidate(word, CANDIDATE_PROBABILITY));

  if (operations_mask_ & ENABLE_CHANGE) {
    for (int i = 0; i < word.size(); ++i) {
      for (char c = 'a'; c <= 'z'; ++c) {
        if (word[i] != c) {
          precandidates.push_back(
              CorrectionCandidate(word.substr(0, i) + c + word.substr(i + 1),
                                  CANDIDATE_PROBABILITY));
        }
      }
    }	
  }

  if (operations_mask_ & ENABLE_INSERT) {
    for (int i = 0; i <= word.size(); ++i) {
      for (char c = 'a'; c <= 'z'; ++c) {
        precandidates.push_back(
            CorrectionCandidate(word.substr(0, i) + c + word.substr(i),
                                CANDIDATE_PROBABILITY));
      }
    }	
  }

  if (operations_mask_ & ENABLE_SWAP) {
    for (int i = 0; i + 1 < word.size(); ++i) {
      if (word[i] != word[i + 1]) {
        precandidates.push_back(
            CorrectionCandidate(word.substr(0, i) +
                                word[i + 1] +
                                word[i] + 
                                word.substr(i + 2),
                                CANDIDATE_PROBABILITY));
      }
    }	
  }

  if (operations_mask_ & ENABLE_DELETE) {
    for (int i = 0; i < word.size(); ++i) {
      precandidates.push_back(
          CorrectionCandidate(word.substr(0, i) + word.substr(i + 1),
                              CANDIDATE_PROBABILITY));
    }	
  }

  for (int i = 0; i < precandidates.size(); ++i) {
    if (dictionary_.GetWordFrequency(precandidates[i].text_) != 0) {
      candidates.push_back(precandidates[i]);
      candidates.back().weight_ *= dictionary_.GetWordFrequency(precandidates[i].text_);
    }
  }
  
  sort(candidates.rbegin(), candidates.rend());
}

