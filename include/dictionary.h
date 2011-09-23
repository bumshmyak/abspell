#ifndef _ABSPELL_DICTIONARY_H_
#define _ABSPELL_DICTIONARY_H_

#include <string>
#include <vector>

class IDictionary {
 public:
  virtual void LoadFromFile(const std::string& filename);
  virtual void LoadFromCollection(const std::vector<std::string>& collection);
  
  virtual void AddWord(const std::string& word);
  virtual void RemoveWord(const std::string& word);
  virtual void GetWordFrequency(const std::string& word);
  virtual int GetSize();

  // something to iterate throu all words

  virtual ~IDictionary() {};
};

#endif  // _ABSPELL_DICTIONARY_H_
