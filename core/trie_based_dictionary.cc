#include <trie_based_dictionary.h>

void TTrieDictionary::LoadFromFile(const std::string& filename) {
  ifstream is(filename.c_str());
  Load(is);
}

void TTrieDictionary::Load(istream& is) {
  size_t line_counter = 0;
  while(!is.eof()) {
    string line;
    getline(is, line);
    vector<string> fields;
    boost::split(fields, line, boost::is_any_of("\t"));

    if (fields.size() < 2) {
      cerr << "skipping line: " << line_counter << endl;
      continue;
    }

    size_t frequency = atoi(fields[0].c_str());
    const string& word = fields[1];

    trie_.add(word, next_word_id_);
    word_attribute_dict_[next_word_id_] = TWordAttribute(fields[1], frequency);
    total_frequency_ += frequency;

    ++line_counter;
    ++next_word_id_;
  }
}

void TTrieDictionary::AddWord(const std::string& word, size_t frequency) {
  trie_.add(word, next_word_id_);
  word_attribute_dict_[next_word_id_] = TWordAttribute(word, frequency);
  total_frequency_ += frequency;
  ++next_word_id_;
}

void TTrieDictionary::SetWordFrequency(
    const std::string& word,
    const double& frequency) {
  size_t word_id = trie_.find(word);

  size_t& old_frequency = word_attribute_dict_.find(word_id)->second.frequency_;
  total_frequency_ -= old_frequency;
  old_frequency = frequency;
  total_frequency_ += old_frequency;
}

size_t TTrieDictionary::GetWordFrequency(const std::string& word) const {
  size_t word_id = trie_.find(word);
  return word_attribute_dict_.find(word_id)->second.frequency_;
}

size_t TTrieDictionary::GetTotalFrequency() const {
  return total_frequency_;
}

size_t TTrieDictionary::GetTotalWordCount() const {
  return word_attribute_dict_.size();
}

void TTrieDictionary::GetNeighbourWords(
    const std::string& word,
    vector<string>* suggestions_ptr) const {
  vector<size_t> word_id_set;
  trie_.find_nearby(word, back_inserter(word_id_set), max_distance_);

  for (size_t index = 0; index < word_id_set.size(); ++index) {
    word_attribute_dict_t::const_iterator it =
        word_attribute_dict_.find(word_id_set[index]);

    if (it == word_attribute_dict_.end()) {
      cerr << "bad word_id: " << word_id_set[index] << endl;
    } else {
      suggestions_ptr->push_back(it->second.word_);
    }
  }
}

size_t TTrieDictionary::GetMaxDistance() const {
  return max_distance_;
}

void TTrieDictionary::SetMaxDistance(size_t max_distance) {
  max_distance_ = max_distance;
}



