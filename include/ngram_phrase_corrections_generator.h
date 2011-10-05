// takes corrections for each word in phrase
// combines word corrections into phrase corrections
// rank result weights using bigram dictionary
#pragma once
#include <using_std.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <corrections_generator.h>
#include <cmath>

class NgramPhraseCorrector :
public IPhraseCandidatesGenerator {

 public:

 NgramPhraseCorrector(const IWordCandidatesGenerator& WordCorrector, double WordPenalty, double Coef) 
   : WordCorrector_(WordCorrector), 
     WordPenalty_(WordPenalty),
     Coef_(Coef) {
  }

  void GetCandidates(
		     const std::string& Phrase,
		     std::vector<CorrectionCandidate>& Candidates) const;

  // return all possible combinations
  void CombineWords(
		    const vector<vector<CorrectionCandidate> >& AllWords,
		    vector<vector<CorrectionCandidate> >* Phrases) const;

  double CalculateWeight(const vector<CorrectionCandidate>& Phrase) const;
 
  double CalculatePhraseWeight(const vector<CorrectionCandidate>& Phrase) const;

  double GetBigramWeight(const CorrectionCandidate& Prev,
					       const CorrectionCandidate& Next) const;

  void PrepareBigramDict(const string Filename);

 private:
  const IWordCandidatesGenerator& WordCorrector_;
  const double WordPenalty_;
  map<pair<string, string>, double> BigramDict;
  //coef
  double Coef_;
};

