/*!
 *  Split phrase into words, replace each word with its most probable correction
 */
#pragma once
#include <cmath>
#include <using_std.h>
#include <boost/algorithm/string.hpp>
#include <corrections_generator.h>

class BestWordPhraseCandidatesGenerator {
 public:
  BestWordPhraseCandidatesGenerator(
      const IWordCandidatesGenerator& word_corrector)
      : word_corrector_(word_corrector) {
  }

  void GetCandidates(
      const std::string& phrase,
      std::vector<CorrectionCandidate>& candidates) const {

    vector<string> words;
    boost::split(words, phrase, boost::is_any_of(" \n\t"));

    vector<CorrectionCandidate> phrase_variants(1, CorrectionCandidate("", 0.0));
    for (size_t index = 0; index < words.size(); ++index) {
      vector<CorrectionCandidate> word_candidates;
      word_corrector_.GetCandidates(words[index], word_candidates, 1.0);
      
      if (word_candidates.size() == 0) {
        for (int i = 0; i < phrase_variants.size(); ++i) {
          phrase_variants[i].text_ += words[index] + " ";
          phrase_variants[i].weight_ += 0.0;
        }
      } else if (word_candidates.size() == 1) {
        for (int i = 0; i < phrase_variants.size(); ++i)
          phrase_variants[i].text_ += word_candidates[0].text_ + " ";
      } else {
        vector<CorrectionCandidate> phrase_variants_new = phrase_variants;
        for (int i = 0; i < phrase_variants.size(); ++i) {
          phrase_variants[i].text_ += word_candidates[0].text_ + " ";
          phrase_variants[i].weight_ += word_candidates[0].weight_;
        }
        if (word_candidates[0].weight_ - word_candidates[1].weight_ < 1) {
          for (int i = 0; i < phrase_variants_new.size(); ++i) {
            phrase_variants_new[i].text_ += word_candidates[1].text_ + " ";
            phrase_variants_new[i].weight_ += word_candidates[1].weight_;
          }
          copy(phrase_variants_new.begin(), phrase_variants_new.end(), back_inserter(phrase_variants));
        }
      }
    }

    double max_weight = -100500 ;
    for (int i = 0; i < phrase_variants.size(); ++i) {
      string& out = phrase_variants[i].text_;
      out = out.substr(0, out.size() - 1);
      max_weight = max(max_weight, phrase_variants[i].weight_);
    }

    bool phrase_found = false;
    for (int i = 0; i < phrase_variants.size(); ++i) {
      double weight = exp(10.3 * (phrase_variants[i].weight_ - max_weight));
      if (weight > 0.00001) {
        candidates.push_back(phrase_variants[i]);
        candidates.back().weight_ = weight;
        if (candidates.back().text_ == phrase) {
          phrase_found = true;
        }
      }
    }
    if (!phrase_found) {
      candidates.push_back(CorrectionCandidate(phrase, 0.0001));
    }

  }

 public:
  const IWordCandidatesGenerator& word_corrector_;
};

