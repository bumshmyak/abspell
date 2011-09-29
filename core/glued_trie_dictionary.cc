#include <using_std.h>
#include <trie_based_dictionary.h>

void TGluedTrieDictionary::LoadFromFile(const std::string& filename) {
  ifstream is(filename.c_str());
  Load(is);
}

void TGluedTrieDictionary::Load(istream& is) {
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

    size_t word_id = next_word_id_;
    if (!trie_.add(encoder_(word), next_word_id_)) {
      word_id = trie_.find(encoder_(word));
    }
    word_frequency_dict_t::iterator freq_it = word_frequency_dict_.find(word);
    if (freq_it == word_frequency_dict_.end()) {
      word_frequency_dict_[word] = frequency;
      word_attribute_dict_.insert(make_pair(word_id, TWordAttribute(fields[1], frequency)));
      total_frequency_ += frequency;
      if (word_id == next_word_id_) {
        ++next_word_id_;
      }
    } // else skipp repeated word

    ++line_counter;
  }
}

void TGluedTrieDictionary::AddWord(const std::string& word, size_t frequency) {
  size_t word_id = next_word_id_;
  if(!trie_.add(encoder_(word), next_word_id_)) {
    word_id = trie_.find(encoder_(word));
  }
  word_frequency_dict_t::iterator freq_it = word_frequency_dict_.find(word);
  if (freq_it == word_frequency_dict_.end()) {
    word_frequency_dict_[word] = frequency;
    word_attribute_dict_.insert(make_pair(word_id, TWordAttribute(word, frequency)));
    total_frequency_ += frequency;
    if (word_id == next_word_id_) {
      ++next_word_id_;
    }
  }
}

void TGluedTrieDictionary::SetWordFrequency(
    const std::string& word,
    const double& frequency) {
  size_t& old_frequency = word_frequency_dict_.find(word)->second;
  total_frequency_ -= old_frequency;
  old_frequency = frequency;
  total_frequency_ += old_frequency;
}

size_t TGluedTrieDictionary::GetWordFrequency(const std::string& word) const {
  return word_frequency_dict_.find(word)->second;
}

size_t TGluedTrieDictionary::GetTotalFrequency() const {
  return total_frequency_;
}

size_t TGluedTrieDictionary::GetTotalWordCount() const {
  return word_attribute_dict_.size();
}

void TGluedTrieDictionary::GetNeighbourWords(
    const std::string& word,
    vector<string>* suggestions_ptr) const {
  vector<size_t> word_id_set;
  string encoded_word = encoder_(word);
  trie_.find_nearby(encoded_word, back_inserter(word_id_set), max_distance_);

  for (size_t index = 0; index < word_id_set.size(); ++index) {
    word_attribute_dict_t::const_iterator
        it = word_attribute_dict_.lower_bound(word_id_set[index]);
    word_attribute_dict_t::const_iterator
        end = word_attribute_dict_.upper_bound(word_id_set[index]);
  
    for (; it != end; ++it) {
      suggestions_ptr->push_back(it->second.word_);
    } 
  }
}

size_t TGluedTrieDictionary::GetMaxDistance() const {
  return max_distance_;
}

void TGluedTrieDictionary::SetMaxDistance(size_t max_distance) {
  max_distance_ = max_distance;
}



