/*
 * unit test for distance functions
 */

#include <gtest/gtest.h>

#include <string>
#include <iostream>
#include <cmath>

#include <distance.h>
#include <hash_based_dictionary.h>
#include <levenshtein_corrections_generator.h>

using std::string;

int lev(const string& lhs, const string& rhs, int max_distance = 0) {
  return get_levenshtein_distance(lhs, rhs, max_distance);
}

TEST(DistanceMetrics, Levenshtein) {
  EXPECT_EQ(0, lev("abcdef", "abcdef", 3));

  EXPECT_EQ(1, lev("abcdef", "bbcdef", 3));
  EXPECT_EQ(1, lev("abcdef", "bcdef", 3));
  EXPECT_EQ(1, lev("abcdef", "acdef", 3));
  EXPECT_EQ(1, lev("abcdef", "abdef", 3));
  EXPECT_EQ(1, lev("abcdef", "abcef", 3));
  EXPECT_EQ(1, lev("abcdef", "abcdf", 3));
  EXPECT_EQ(1, lev("abcdef", "abcde", 3));

  EXPECT_EQ(1, lev("aaaa", "aaaaa", 3));
  
  EXPECT_EQ(0, lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj",
                   "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj"));
  EXPECT_EQ(3, lev("axsjdioqwdiqwhoqjhfiojalfaslfjasklfjklasdfj",
                   "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj"));
  EXPECT_EQ(3, lev("asjdioqdiqwhoqjdfiojlaslfjasklfjklasdfj",
                   "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj"));
  EXPECT_EQ(3, lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj",
                   "asjdioqdiqwhoqjdhfiojalfassdlfjaskljklasdfj"));
  EXPECT_EQ(1, lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj",
                "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdf"));
  EXPECT_EQ(2, lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj",
                   "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasj"));
  EXPECT_EQ(3, lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj",
                   "asjdioqdiqwhoqjdhfiojalfaslfjasklfjkdfj"));
  EXPECT_EQ(3, lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasfj",
                   "ajdioqdiqwhoqjdhfojalfaslfjasklfjklasdfj"));

  EXPECT_EQ(1, lev(string(100, 'c'), string(101, 'c'), 3));
  EXPECT_EQ(101, lev(string(100, 'a'), string(101, 'c'), 3));
}

TEST(DistanceMetrics, SimpleLevenshtein) {
  THashDictionary hash_dictionary;
  hash_dictionary.AddWord("hel");      
  hash_dictionary.AddWord("held");
  hash_dictionary.AddWord("hal");
  hash_dictionary.AddWord("he");
  hash_dictionary.AddWord("hle");

  SimpleLevenshteinWordCandidatesGenerator candidates_generator(
      hash_dictionary);

  vector<CorrectionCandidate> candidates;
  candidates_generator.GetCandidates("hel", candidates, 1);
  EXPECT_EQ(5, candidates.size());
}

TEST(DistanceMetrics, NGrammBased) {
  EXPECT_EQ(0, get_ngramm_jaccard_distance("abc", "abc"));
  EXPECT_EQ(0, get_ngramm_dice_distance("abc", "abc"));
  
  EXPECT_DOUBLE_EQ(1.0 - 2.0 / 5, get_ngramm_jaccard_distance("abc", "bc"));
  EXPECT_DOUBLE_EQ(1.0 - 4.0 / 7, get_ngramm_dice_distance("abc", "bc"));
  
  EXPECT_EQ(1.0, get_ngramm_jaccard_distance("", ""));
  EXPECT_EQ(1.0, get_ngramm_dice_distance("", ""));

  EXPECT_EQ(1.0, get_ngramm_jaccard_distance("anything", ""));
  EXPECT_EQ(1.0, get_ngramm_dice_distance("", "anything"));
}

