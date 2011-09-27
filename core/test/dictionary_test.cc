#include <gtest/gtest.h>

#include <string>
#include <iostream>
#include <stdexcept>

#include <hash_based_dictionary.h>

using std::string;

TEST(DictionaryTest, HashBased) {
  THashDictionary hash_dictionary;
  
  bool open_file_exception_happens = false;
  try {
    hash_dictionary.LoadFromFile("../../data/top1000_wordlist_coca.dat");
  } catch(const std::runtime_error& e) {
    open_file_exception_happens = true;
  }
  ASSERT_FALSE(open_file_exception_happens);

  // LE cause one word can by metioned multiple times in the file as
  // different part of speech
  EXPECT_LE(22995878, hash_dictionary.GetWordFrequency("the"));
  EXPECT_LE(9247774, hash_dictionary.GetWordFrequency("a"));
  EXPECT_LE(40395, hash_dictionary.GetWordFrequency("call"));
  EXPECT_EQ(0, hash_dictionary.GetWordFrequency("verystrangeword"));  
}
