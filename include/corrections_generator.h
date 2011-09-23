#ifndef _ABSPELL_CORRECTIONS_GENERATOR_H_
#define _ABSPELL_CORRECTIONS_GENERATOR_H_

#include <string>
#include <vector>

#include "dictionary.h"

struct CorrectionCandidate {
  std::string candidate;
  double weight; // probability
};

class IWordCandidatesGenerator {
 public:
  // generates all word correction candidates with distance <= threshold
  // by some metric
  virtual void GetCandidates(
      const std::string& word,
      const IDictionary* dictionary,
      std::vector<CorrectionCandidate>& candidates,
      double threshold);
  virtual ~IWordCandidatesGenerator() {};
};

class IPhrazeCandidatesGenerator {
 public:
  // one possible algo for this:
  // split phraze into words
  // get word corrections for each word
  // combine word corrections to get phraze correction
  // rank phraze corrections
  virtual void GetCandidates(
      const std::string& phraze,
      std::vector<CorrectionCandidate>& candidates);

  virtual ~IPhrazeCandidatesGenerator() {};
};

#endif  // _ABSPELL_CORRECTORIONS_GENERATOR_H_
