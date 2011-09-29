#pragma once
#include <using_std.h>

#include <boost/algorithm/string.hpp>
#include <trie.h>
#include <dictionary.h>
#include <tr1/unordered_map>

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
class TTrieDictionary :
    public ISuggestingDictionary {
 public:
  TTrieDictionary()
      : max_distance_(0),
        next_word_id_(1),
        total_frequency_(0)
  {}

  virtual void LoadFromFile(const std::string& filename);
  virtual void AddWord(const std::string& word, size_t frequency = 1);

  void SetWordFrequency(const std::string& word, const double& frequency);

  virtual size_t GetWordFrequency(const std::string& word) const;
  virtual size_t GetTotalFrequency() const;
  virtual size_t GetTotalWordCount() const;
  virtual void GetNeighbourWords(
      const std::string& word,
      vector<string>* suggestions_ptr) const;

  size_t GetMaxDistance() const;
  void SetMaxDistance(size_t max_distance);
private:
  void Load(istream& is);

  typedef std::tr1::unordered_map<size_t, TWordAttribute> word_attribute_dict_t;

  size_t max_distance_;
  size_t next_word_id_;
  size_t total_frequency_;
  TTrie<size_t> trie_;
  word_attribute_dict_t word_attribute_dict_;
};


class IStringEncoder {
 public:
  virtual string operator()(const string& text) const = 0;
};

class TDummyEncoder {
 public:
  string operator()(const string& text) const {
    return text;
  }
};

class TGluedTrieDictionary :
    public ISuggestingDictionary {
 public:
  TGluedTrieDictionary(const IStringEncoder& encoder)
      : max_distance_(0),
        next_word_id_(1),
        total_frequency_(0),
        encoder_(encoder)
  {}

  virtual void LoadFromFile(const std::string& filename);
  virtual void AddWord(const std::string& word, size_t frequency = 1);

  void SetWordFrequency(const std::string& word, const double& frequency);

  virtual size_t GetWordFrequency(const std::string& word) const;
  virtual size_t GetTotalFrequency() const;
  virtual size_t GetTotalWordCount() const;
  virtual void GetNeighbourWords(
      const std::string& word,
      vector<string>* suggestions_ptr) const;

  size_t GetMaxDistance() const;
  void SetMaxDistance(size_t max_distance);

private:
  void Load(istream& is);

  typedef std::multimap<size_t, TWordAttribute> word_attribute_dict_t;
  typedef std::tr1::unordered_map<string, size_t> word_frequency_dict_t;

  size_t max_distance_;
  size_t next_word_id_;
  size_t total_frequency_;
  TTrie<size_t> trie_;
  word_attribute_dict_t word_attribute_dict_;
  word_frequency_dict_t word_frequency_dict_;
  const IStringEncoder& encoder_;
};
