#include <hash_based_dictionary.h>

#include <boost/algorithm/string.hpp>

void THashDictionary::LoadFromFile(const std::string& filename) {
  ifstream in(filename.c_str());
  if (!in.is_open()) {
    throw std::runtime_error("can't open file");
  }
  string line;
  while (getline(in, line)) {
    vector<string> fields;
    boost::split(fields, line, boost::is_any_of("\t"));
    if (fields.size() != 4) {
      // cerr << "skipping line: " << line << " --- " << fields.size() << endl;
      continue;
    }
    int frequency = atoi(fields[0].c_str());
    dict_[fields[1]] += frequency;
    ++total_count_;
    total_frequency_ += frequency; 
  }
}
  
  
void THashDictionary::AddWord(const std::string& word) {
  ++dict_[word];
}

size_t THashDictionary::GetWordFrequency(const std::string& word) const {
  std::tr1::unordered_map<string, int>::const_iterator word_iterator
      = dict_.find(word);
  return (word_iterator == dict_.end()) ? 0 : word_iterator->second;
}

size_t THashDictionary::GetSize() const {
  return total_frequency_;
}

void THashDictionary::GetNeighbourWords(
    const std::string& word,
    vector<string>* suggestions_ptr) const {
}


