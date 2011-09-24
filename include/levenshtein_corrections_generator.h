#ifndef _ABSPELL_LEVENSHTEIN_CORRECTIONS_GENERATOR_
#define _ABSPELL_LEVENSHTEIN_CORRECTIONS_GENERATOR_

#include <string>
#include <vector>

#include <distance.h>
#include "corrections_generator.h"

class LevenshteinWordCandidatesGenerator :
    public IWordCandidatesGenerator {
 
 public:
  explicit LevenshteinWordCandidatesGenerator(
      const IDictionary& dictionary)  
      : dictionary_(dictionary) {
  }

  void GetCandidates(
      const std::string& word,
      std::vector<CorrectionCandidate>& candidates,
      double threshold) const;
    
 private:
  const IDictionary& dictionary_;
};


#endif  // _ABSPELL_LEVENSHTEIN_CORRECTIONS_GENERATOR_
