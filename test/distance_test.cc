/*
 * unit test for distance functions
 */

#include <gtest/gtest.h>

#include <string>
#include <iostream>

#include <distance.h>

using std::string;

int lev(const string& lhs, const string& rhs, int max_distance = 0) {
    return get_levenshtein_distance(lhs, rhs, max_distance);
}

TEST(DistanceMetrics,Levenshtein) {

  EXPECT_EQ(lev("abcdef", "abcdef", 3), 0);

  EXPECT_EQ(lev("abcdef", "bbcdef", 3), 1);
  EXPECT_EQ(lev("abcdef", "bcdef", 3), 1);
  EXPECT_EQ(lev("abcdef", "acdef", 3), 1);
  EXPECT_EQ(lev("abcdef", "abdef", 3), 1);
  EXPECT_EQ(lev("abcdef", "abcef", 3), 1);
  EXPECT_EQ(lev("abcdef", "abcdf", 3), 1);
  EXPECT_EQ(lev("abcdef", "abcde", 3), 1);

  EXPECT_EQ(lev("aaaa", "aaaaa", 3), 1);
  
  EXPECT_EQ(lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj", "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj"), 0);
  EXPECT_EQ(lev("axsjdioqwdiqwhoqjhfiojalfaslfjasklfjklasdfj", "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj"), 3);
  EXPECT_EQ(lev("asjdioqdiqwhoqjdfiojlaslfjasklfjklasdfj", "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj"), 3);
  EXPECT_EQ(lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj", "asjdioqdiqwhoqjdhfiojalfassdlfjaskljklasdfj"), 3);
  EXPECT_EQ(lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj", "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdf"), 1);
  EXPECT_EQ(lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj", "asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasj"), 2);
  EXPECT_EQ(lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasdfj", "asjdioqdiqwhoqjdhfiojalfaslfjasklfjkdfj"), 3);
  EXPECT_EQ(lev("asjdioqdiqwhoqjdhfiojalfaslfjasklfjklasfj", "ajdioqdiqwhoqjdhfojalfaslfjasklfjklasdfj"), 3);

  EXPECT_EQ(lev(string(100, 'c'), string(101, 'c'), 3), 1);
  EXPECT_EQ(lev(string(100, 'a'), string(101, 'c'), 3), 101);
}

/*
TEST(DistanceMetrics,NGrammBased) {
  std::cout << get_ngramm_distance("abcdef", "abcdef", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "bcdef", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "acdef", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "abdef", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "abcef", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "abcdf", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "abcde", 2) << std::endl; 
  
  std::cout << get_ngramm_distance("abcdef", "bacdef", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "acbdef", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "abdcef", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "abcedf", 2) << std::endl; 
  std::cout << get_ngramm_distance("abcdef", "abcdfe", 2) << std::endl; 
}
*/
