#pragma once

#include <using_std.h>
#include <dictionary.h>
#include <tr1/unordered_map>

class THashDictionary :
    public IDictionary {
 public:
  virtual void LoadFromFile(const std::string& filename);
  
  virtual void LoadFromCollection(const std::vector<std::string>& collection) {};
  
  virtual void AddWord(const std::string& word);
  virtual void RemoveWord(const std::string& word) {};
  virtual size_t GetWordFrequency(const std::string& word) const;
  virtual size_t GetSize() const;
  virtual void GetNeighbourWords(const std::string& word,
                                 vector<string>* suggestions_ptr) const;
 private:
  std::tr1::unordered_map<string, int> dict_;
  size_t total_count_;
  size_t total_frequency_;
};
