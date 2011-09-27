#pragma once
#include <string>
#include <vector>

#include "dictionary.h"

struct CorrectionCandidate {
  CorrectionCandidate(const std::string& text, const double& weight)
    : text_(text)
    , weight_(weight) {
  }
 
  bool operator < (const CorrectionCandidate& rhs) const {
    return weight_ < rhs.weight_;
  }

 public:
  std::string text_;
  double weight_; // probability
};

class IWordCandidatesGenerator {
 public:
  // generates all word correction candidates with distance <= threshold
  // by some metric
  // Dictionary presence and it's interface could depend on WordCandidatesGenerator realization
  virtual void GetCandidates(
      const std::string& word,
      std::vector<CorrectionCandidate>& candidates,
      double threshold) const = 0;

  virtual ~IWordCandidatesGenerator() {};
};

class IPhraseCandidatesGenerator {
 public:
  // one possible algo for this:
  // split phraze into words
  // get word corrections for each word
  // combine word corrections to get phraze correction
  // rank phraze corrections
  virtual void GetCandidates(
      const std::string& phraze,
      std::vector<CorrectionCandidate>& candidates) const = 0;

  virtual ~IPhraseCandidatesGenerator() {};
};

