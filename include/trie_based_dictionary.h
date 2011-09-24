#pragma once

#include <using_std.h>

#include <boost/algorithm/string.hpp>

#include <trie.h>
#include <dictionary.h>

struct TWordAttribute {
  TWordAttribute()
      : word_()
      , frequency_(0) {
  }
  TWordAttribute(string word, size_t frequency) 
      : word_(word)
      , frequency_(frequency) {
  }

  string word_;
  size_t frequency_;
};

/* 
 *  Dictionary on trie with neighbour search function;
 *
 */
class TTrieDictionary : public IDictionary {
 public:
  TTrieDictionary()
    : max_distance_(0)
    , next_word_id_(1)
  {}

  void LoadFromFile(const std::string& filename) {
    ifstream is(filename.c_str());
    Load(is);
  }

  void LoadFromCollection(const std::vector<std::string>& collection) {};
 
  void Load(istream& is) {
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
         
      trie_.add(fields[1], next_word_id_);
      word_attribute_dict_[next_word_id_] = TWordAttribute(fields[1], atoi(fields[0].c_str()));

      ++line_counter;
      ++next_word_id_;
    }
  }

  void AddWord(const std::string& word) {
    trie_.add(word, next_word_id_);
    word_attribute_dict_[next_word_id_] = TWordAttribute(word, 0);
    ++next_word_id_;
  }

  void RemoveWord(const std::string& word) {};

  void SetWordFrequency(const std::string& word, const double& frequency) {
    size_t word_id = trie_.find(word);
    word_attribute_dict_.find(word_id)->second.frequency_ = frequency;
  }
  size_t GetWordFrequency(const std::string& word) const {
    size_t word_id = trie_.find(word);
    return word_attribute_dict_.find(word_id)->second.frequency_;
  }

  size_t GetSize() const {
    return word_attribute_dict_.size();
  }

  void GetNeighbourWords(const std::string& word, vector<string>* suggestions_ptr) const {
    vector<size_t> word_id_set;
    trie_.find_nearby(word, back_inserter(word_id_set), max_distance_);

    for (size_t index = 0; index < word_id_set.size(); ++index) {
      map<size_t, TWordAttribute>::const_iterator it = word_attribute_dict_.find(word_id_set[index]);
      
      if (it == word_attribute_dict_.end()) {
        cerr << "bad word_id: " << word_id_set[index] << endl; 
      } else {
        suggestions_ptr->push_back(it->second.word_);
      }
    }
  }

  size_t GetMaxDistance() const {
    return max_distance_;
  } 
  
  void SetMaxDistance(size_t max_distance) {
    max_distance_ = max_distance;
  } 

private:
  size_t max_distance_;
  size_t next_word_id_;
  TTrie<size_t> trie_;
  map<size_t, TWordAttribute> word_attribute_dict_;
};



