#include <using_std.h>

#include <cmath>

#include <boost/assign.hpp>

#include "levenshtein_corrections_generator.h"

using std::string;
using std::vector;

bool has_non_alphabetic_symbols(const string& word) {
 for (int i = 0; i < word.size(); ++i) {
    if (word[i] < 'a' || word[i] > 'z') {
      return true;
    }
  }
  return false;
}

void LevenshteinWordCandidatesGenerator::GetCandidates(
    const string& word,
    vector<CorrectionCandidate>& candidates,
    double threshold) const {

  // dont correct chort words
  if (word.size() < 6) {
    candidates.push_back(CorrectionCandidate(word, 1.0));
    return;
  }

  if (has_non_alphabetic_symbols(word)) {
    candidates.push_back(CorrectionCandidate(word, 1.0));
    return;
  }

  vector<string> candidate_words;
  dictionary_.GetNeighbourWords(word, &candidate_words);

  vector<CorrectionCandidate> candidates_unsorted;
  double cost_matrix[] = {0.45, 1.8, 1.8, 1.1, 0.8, 2.0};
  for (size_t index = 0; index < candidate_words.size(); ++index) {
    double levenshtein_distance =
        get_weighted_levenshtein_distance(word, candidate_words[index], 10, cost_matrix);
    //double misspell_probability_ =
    //    0.15 - levenshtein_distance / max(word.size(), candidate_words[index].size());

    double candidate_frequency_ =
        dictionary_.GetWordFrequency(candidate_words[index]);

    candidates_unsorted.push_back(
        CorrectionCandidate(
          candidate_words[index],
          log(candidate_frequency_ + 1.0) - 7.0 * levenshtein_distance));
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
  // evristics:
  // don't correct words with non alphabetic symbols
  // each operation has different cost
  // don't modify first letter of the word
  // don't change or delete last letter of the word
  // tune tradeof between word frequency and levenshtein distance
  // it's not expensive to merge doubled letter or to split one letter in two

  double change_coef = 1.1;
  double doubling_coef = 0.4;
  double levenshtein_coef = 11.5;

  map<string,double> change_weight;
  boost::assign::insert(change_weight) 
      ("ck", 0.8)
      ("kc", 0.8)
      ("cq", 0.8)
      ("qc", 0.8)
      
      ("cs", 0.8)
      ("sc", 0.8)
      ("sz", 0.8)
      ("zs", 0.8)

/*
      ("st", 0.9)
      
      ("rl", 0.9)
      ("lr", 0.9)
  */    
      ("ao", 0.9)
      ("oa", 0.9)
      ("ae", 0.9)
      ("ea", 0.9)
      ("ei", 0.9)
      ("ie", 0.9)
/*      
      ("wu", 0.9)
      ("uw", 0.9)
      ("wv", 0.9)
      ("vw", 0.9)
*/
      ;

  if (has_non_alphabetic_symbols(word)) {
    candidates.push_back(CorrectionCandidate(word, 1.0));
    return;
  }

  vector<CorrectionCandidate> precandidates;

  precandidates.push_back(CorrectionCandidate(word, 0));

  if (operations_mask_ & ENABLE_CHANGE) {
    for (int i = 1; i + 1 < word.size(); ++i) {
      for (char c = 'a'; c <= 'z'; ++c) {
        if (word[i] != c) {
          double cost = change_coef;
          string key(1, word[i]);
          key += c;
          if (change_weight.count(key)) {
            cost *= change_weight[key];
          }
          precandidates.push_back(
              CorrectionCandidate(word.substr(0, i) + c + word.substr(i + 1), cost));
        }
      }

    }	
  }

  if (operations_mask_ & ENABLE_INSERT) {
    for (int i = 1; i <= word.size(); ++i) {
      for (char c = 'a'; c <= 'z'; ++c) {
        double d = (c == word[i]) ? doubling_coef : 1;
        precandidates.push_back(
            CorrectionCandidate(word.substr(0, i) + c + word.substr(i), d));
      }
    }	
  }

  if (operations_mask_ & ENABLE_SWAP) {
    for (int i = 1; i + 1 < word.size(); ++i) {
      if (word[i] != word[i + 1]) {
        precandidates.push_back(
            CorrectionCandidate(word.substr(0, i) +
                                word[i + 1] +
                                word[i] +
                                word.substr(i + 2),
                                1));
      }
    }	
  }

  if (operations_mask_ & ENABLE_DELETE) {
    for (int i = 1; i + 1 < word.size(); ++i) {
      double d = (word[i] == word[i + 1]) ? doubling_coef : 1;
      precandidates.push_back(
          CorrectionCandidate(word.substr(0, i) + word.substr(i + 1), d));
    }	
  }

  for (int i = 0; i < precandidates.size(); ++i) {
    if (dictionary_.GetWordFrequency(precandidates[i].text_) != 0) {
      candidates.push_back(precandidates[i]);
      candidates.back().weight_ =
          log(dictionary_.GetWordFrequency(precandidates[i].text_) + 1) -
          levenshtein_coef * candidates.back().weight_;
    }
  }

  double compo_freq = dictionary_.GetWordFrequency(word);
  if (compo_freq < 2) {
    for (int i = 4; i + 4 < word.size(); ++i) {
      double first_freq = dictionary_.GetWordFrequency(word.substr(0,i));
      double second_freq = dictionary_.GetWordFrequency(word.substr(i));
      if (min(first_freq, second_freq) > 17000) {
        candidates.push_back(
            CorrectionCandidate(
              word.substr(0, i) + ' ' + word.substr(i), 
              log(sqrt(first_freq*second_freq))));
      }
    }
  }

  sort(candidates.rbegin(), candidates.rend());
}

