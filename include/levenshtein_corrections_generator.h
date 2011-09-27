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


class SimpleLevenshteinWordCandidatesGenerator :
    public IWordCandidatesGenerator {
 
 public:
  enum EnabledOperationsType {
    ENABLE_CHANGE = 0x1,
    ENABLE_SWAP = 0x2,
    ENABLE_INSERT = 0x4,
    ENABLE_DELETE = 0x8,
    ENABLE_ALL = 0xFFFF
  };
  
  explicit SimpleLevenshteinWordCandidatesGenerator(
      const IDictionary& dictionary,
      size_t operations_mask = ENABLE_ALL)  
      : dictionary_(dictionary),
        operations_mask_(operations_mask) {
  }

  void GetCandidates(
      const std::string& word,
      std::vector<CorrectionCandidate>& candidates,
      double threshold /* not used right now */ ) const;
    
 private:
  const IDictionary& dictionary_;
  size_t operations_mask_;
};


#endif  // _ABSPELL_LEVENSHTEIN_CORRECTIONS_GENERATOR_
