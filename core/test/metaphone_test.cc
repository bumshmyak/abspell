#include <gtest/gtest.h>
#include <dmetaph.h>

TEST(Metaphone, version1) {
  EXPECT_EQ("ALTRNTF", metaphone("alternative"));
  EXPECT_EQ("AMPLMNTXN", metaphone("implementation"));
  EXPECT_EQ("JNRXN", metaphone("generation"));
  EXPECT_EQ("AMFSS", metaphone("emphasis"));
  EXPECT_EQ("AKRTNK", metaphone("according"));
  EXPECT_EQ("STSFKXN", metaphone("satisfaction"));
}

/* // return some stuff instead of sensible data
TEST(Metaphone, version2) {
  EXPECT_EQ("ALTRNTF", metaphone2("alternative"));
  EXPECT_EQ("AMPLMNTXN", metaphone2("implementation"));
  EXPECT_EQ("JNRXN", metaphone2("generation"));
  EXPECT_EQ("AMFSS", metaphone2("emphasis"));
  EXPECT_EQ("AKRTNK", metaphone2("according"));
  EXPECT_EQ("STSFKXN", metaphone2("satisfaction"));
}
*/
