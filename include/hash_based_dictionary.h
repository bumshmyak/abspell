#pragma once
#include <using_std.h>

#include <dictionary.h>
#include <tr1/unordered_map>

class THashDictionary :
    public IDictionary {
 public:
  THashDictionary() :
      total_count_(0),
      total_frequency_(0)
  { }
  // currently can load only in format of 500k_wordlist_coca_orig
  virtual void LoadFromFile(const std::string& filename);
  virtual void AddWord(const std::string& word, size_t frequency = 1);
  virtual size_t GetWordFrequency(const std::string& word) const;
  virtual size_t GetTotalFrequency() const;
  virtual size_t GetTotalWordCount() const;
 private:
  std::tr1::unordered_map<string, int> dict_;
  size_t total_count_;
  size_t total_frequency_;
};
