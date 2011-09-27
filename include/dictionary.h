#ifndef _ABSPELL_DICTIONARY_H_
#define _ABSPELL_DICTIONARY_H_

#include <using_std.h>

class IDictionary {
 public:
  virtual void LoadFromFile(const std::string& filename) = 0;
  virtual void LoadFromCollection(const std::vector<std::string>& collection) {};
  
  virtual void AddWord(const std::string& word) {};
  virtual void RemoveWord(const std::string& word) {};
  virtual size_t GetWordFrequency(const std::string& word) const = 0;
  virtual size_t GetSize() const = 0;

  // something to iterate throu all words

  virtual void GetNeighbourWords(const std::string& word, vector<string>* suggestions_ptr) const = 0;

  virtual ~IDictionary() {};
};

#endif  // _ABSPELL_DICTIONARY_H_
