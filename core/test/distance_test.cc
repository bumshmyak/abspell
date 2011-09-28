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
  int result = get_levenshtein_distance(lhs, rhs, max_distance);
  EXPECT_EQ(result, get_levenshtein_distance(rhs, lhs, max_distance));
  return result;
}

TEST(DistanceMetrics, Levenshtein) {
  EXPECT_EQ(0, lev("abcdef", "abcdef", 3));

  EXPECT_EQ(1, lev("abcdef", "acbdef", 3)); // transpose
  EXPECT_EQ(1, lev("abcdef", "abdcef", 3)); // transpose
  EXPECT_EQ(1, lev("abcdef", "abcedf", 3)); // transpose
  EXPECT_EQ(1, lev("abcdef", "abcdfe", 3)); // transpose

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

double wlev(const string& lhs, const string& rhs, int max_distance = 0) {
  const double cost_matrix[] = {0.5, 1,1,1,1,0};
  double result = 
      get_weighted_levenshtein_distance(lhs, rhs, max_distance + 10, cost_matrix);
  EXPECT_EQ(result,
      get_weighted_levenshtein_distance(rhs, lhs, max_distance + 10, cost_matrix));
  return result;
}

TEST(DistanceMetrics, WeightedLevenshtein) {
  // doubling
  EXPECT_DOUBLE_EQ(0.5, wlev("cinderella",
                             "cinderela"));
  EXPECT_DOUBLE_EQ(0.5, wlev("error",
                             "eror"));
  EXPECT_DOUBLE_EQ(0.5, wlev("upper",
                             "uper"));
 
  EXPECT_DOUBLE_EQ(0.5, wlev("apreciate",
                             "appreciate"));
  EXPECT_DOUBLE_EQ(0.5, wlev("umbrela",
                             "umbrella"));
  EXPECT_DOUBLE_EQ(0.5, wlev("emberasing",
                             "emberrasing"));
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
  EXPECT_EQ(4, candidates.size());
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

TEST(DistanceMetrics, QwertyKeybordDistance) {
  EXPECT_EQ(0, get_qwerty_keybord_distance('d', 'd'));
  EXPECT_EQ(1, get_qwerty_keybord_distance('d', 's'));
  EXPECT_EQ(1, get_qwerty_keybord_distance('d', 'f'));
  EXPECT_EQ(1, get_qwerty_keybord_distance('d', 'e'));
  EXPECT_EQ(1, get_qwerty_keybord_distance('d', 'c'));
  EXPECT_EQ(2, get_qwerty_keybord_distance('d', 'w'));
  EXPECT_EQ(2, get_qwerty_keybord_distance('d', 'r'));
  EXPECT_EQ(2, get_qwerty_keybord_distance('d', 'x'));
  EXPECT_EQ(2, get_qwerty_keybord_distance('d', 'v'));
}
