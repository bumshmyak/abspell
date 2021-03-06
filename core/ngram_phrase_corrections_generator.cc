#include <ngram_phrase_corrections_generator.h>

void NgramPhraseCorrector::GetCandidates(
  const string& Phrase,
  vector<CorrectionCandidate>& Candidates) const {
  
  size_t NumberToShow = 1;

  vector<string> Words;
  boost::split(Words, Phrase, boost::is_any_of(" \n\t"));
  
  vector<vector<CorrectionCandidate> > AllWords;

  for (size_t WordIndex = 0; WordIndex < Words.size(); ++WordIndex) {
    vector<CorrectionCandidate> WordCandidates;
    WordCorrector_.GetCandidates(Words[WordIndex], WordCandidates, WordPenalty_);
    if (!WordCandidates.size()) {
      //there aren't any candidates
      WordCandidates.push_back(CorrectionCandidate(Words[WordIndex], 1));
    }
    AllWords.push_back(WordCandidates);
  }
  
  if (AllWords.size() > 8) {
    // phrase is extremly big
    // keep only 1 candidate for every word
    for (size_t WordIndex = 0; WordIndex < AllWords.size(); ++WordIndex) {
      AllWords[WordIndex] = vector<CorrectionCandidate>(1, AllWords[WordIndex][0]);
    }
  } else {
    // keep only 3 candidates for every word
    for (size_t WordIndex = 0; WordIndex < AllWords.size(); ++WordIndex) {
      if (AllWords[WordIndex].size() > 3) {
	vector<CorrectionCandidate>FewCandidates = 
	  vector<CorrectionCandidate>(AllWords[WordIndex].begin(), AllWords[WordIndex].begin() + 3);
	AllWords[WordIndex] = FewCandidates;
      }
    }
  }

  vector<vector<CorrectionCandidate> > Phrases;
  CombineWords(AllWords, &Phrases);
 
  vector<CorrectionCandidate> PreCandidates;

  for (size_t PhraseIndex = 0; PhraseIndex < Phrases.size(); ++PhraseIndex) {
    double Weight = CalculatePhraseWeight(Phrases[PhraseIndex]);
    string Phrase;
    for (size_t WordIndex = 0; WordIndex < Phrases[PhraseIndex].size(); ++WordIndex) {
      Phrase += Phrases[PhraseIndex][WordIndex].text_;
      Phrase += " ";
      }
    Phrase.erase(Phrase.length() - 1, 1);
    PreCandidates.push_back(CorrectionCandidate(Phrase, Weight));
  }

  sort(PreCandidates.rbegin(), PreCandidates.rend());
 
  if (PreCandidates.size() > NumberToShow) {
    Candidates = vector<CorrectionCandidate>(PreCandidates.begin(), PreCandidates.begin() + NumberToShow);
  } else {
    Candidates = vector<CorrectionCandidate>(PreCandidates);
  }

  // correct weights
  double LastWeight = (Candidates.back()).weight_;
  if (LastWeight < 0) {
    for (size_t Index = 0; Index < Candidates.size(); ++Index) {
      Candidates[Index].weight_ = Candidates[Index].weight_ - LastWeight + 1;
    }
  }
  // increase first weight
  if ((Candidates.size() > 1) && ((Candidates.back()).weight_ / Candidates[0].weight_) < 10) {
    Candidates[0].weight_ = 10 * (Candidates.back()).weight_;
  }
  
 }

void NgramPhraseCorrector::CombineWords(
  const vector<vector<CorrectionCandidate> >& AllWords,
  vector<vector<CorrectionCandidate> >* Phrases) const {

  size_t DuplicateNumber = 1;

  for (size_t WordIndex = 0; WordIndex < AllWords.size(); ++WordIndex) {
    DuplicateNumber *= AllWords[WordIndex].size();
  }

  size_t TotalNumber = DuplicateNumber;

  for (size_t WordIndex = 0; WordIndex < AllWords.size(); ++WordIndex) {
    size_t Index = 0;
    DuplicateNumber /= AllWords[WordIndex].size(); 
    for (size_t SeriesIndex = 0; SeriesIndex < TotalNumber / (AllWords[WordIndex].size() * DuplicateNumber) ; ++SeriesIndex) {
      // series duplicate
      for (size_t VariantIndex = 0; VariantIndex <  AllWords[WordIndex].size(); ++VariantIndex) {
        //symbol duplicate
        for (size_t DuplicateIndex = 0; DuplicateIndex < DuplicateNumber; ++DuplicateIndex) {
          if (!WordIndex) {
            vector<CorrectionCandidate> Empty;
            Empty.push_back(AllWords[WordIndex][VariantIndex]);
            (*Phrases).push_back(Empty);
          } else {
            (*Phrases)[Index].push_back(AllWords[WordIndex][VariantIndex]);
            ++Index;
          }
        }
      }
    }
  }
}

double NgramPhraseCorrector::CalculatePhraseWeight(const vector<CorrectionCandidate>& Phrase) const {
  double Result = 0;
  for (size_t WordIndex = 0; WordIndex < Phrase.size(); ++WordIndex) {
    if (!WordIndex) {
      // first word in phrase
      Result += Phrase[WordIndex].weight_;
    } else {
      Result += GetBigramWeight(Phrase[WordIndex - 1], Phrase[WordIndex]);
    }
  }
  return Result;
}

double NgramPhraseCorrector::GetBigramWeight(const CorrectionCandidate& Prev,
					     const CorrectionCandidate& Next) const {
  map<pair<string, string>, double>::const_iterator ToFind = BigramDict.find(pair<string,string>(Prev.text_, Next.text_));
  if (ToFind != BigramDict.end()) {
    return ToFind->second;
  } else {
    //independent
    return Next.weight_;
  }
}

void NgramPhraseCorrector::PrepareBigramDict(const string Filename) {
 
  ifstream Input;
  Input.open(Filename.c_str(), std::ios::in);

  while (!Input.eof()) {

    string Line;
    getline(Input,Line);
    if (!Line.length()) {
      continue;
    }

    vector<string> Words;
    boost::split(Words, Line, boost::is_any_of(" \n\t"));
    
    BigramDict[pair<string, string>(Words[0], Words[1])] = log(boost::lexical_cast<size_t>(Words[2])) + Coef_;

  }

}
