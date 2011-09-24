/*!
 *  Split phrase into words, replace each word with its most probable correction
 */

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

    stringstream phrase_stream;
    for (size_t index = 0; index < words.size(); ++index) {
      vector<CorrectionCandidate> word_candidates;
      word_corrector_.GetCandidates(words[index], word_candidates, 1.0);
      
      if (word_candidates.size() == 0) {
        phrase_stream << words[index] << " ";
      } else {
        phrase_stream << word_candidates[0].text_ << " ";
      }
    }

    string out = phrase_stream.str();
    candidates.push_back(CorrectionCandidate(out.substr(0, out.size() - 1), 1.0));
  }

 public:
  const IWordCandidatesGenerator& word_corrector_;
};

