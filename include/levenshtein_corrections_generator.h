#ifndef _ABSPELL_LEVENSHTEIN_CORRECTIONS_GENERATOR_
#define _ABSPELL_LEVENSHTEIN_CORRECTIONS_GENERATOR_

#include <string>
#include <vector>

#include "corrections_generator.h"

class LevenshteinWordCandidatesGenerator :
    public IWordCandidatesGenerator {
 public:
  virtual void GetCandidates(
      const std::string& word,
      const IDictionary* dictionary,
      std::vector<CorrectionCandidate>& candidates,
      double threshold);
};


#endif  // _ABSPELL_LEVENSHTEIN_CORRECTIONS_GENERATOR_
