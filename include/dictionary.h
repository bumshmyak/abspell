#pragma once
#include <using_std.h>

class IDictionary {
 public:
  virtual void LoadFromFile(const std::string& filename) = 0;
  virtual void AddWord(const std::string& word, size_t frequency) = 0;
  virtual size_t GetWordFrequency(const std::string& word) const = 0;
  virtual size_t GetTotalFrequency() const = 0;
  virtual size_t GetTotalWordCount() const = 0;

  virtual ~IDictionary() {};
};

class ISuggestingDictionary :
    public IDictionary {
 public:
  virtual void GetNeighbourWords(
      const std::string& word,
      vector<string>* suggestions_ptr) const = 0;
  virtual ~ISuggestingDictionary() {};
};
