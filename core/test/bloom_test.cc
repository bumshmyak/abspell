
#include <gtest/gtest.h>
#include <bloom.h>

TEST(bloom_filter_Test,StoreStrings) {
  bloom_filter filter(1024);

  filter.add("hello");
  filter.add("apple");
  filter.add("morning");
  filter.add("night");

  EXPECT_EQ(true, filter.find("hello"));
  EXPECT_EQ(true, filter.find("apple"));
  EXPECT_EQ(true, filter.find("morning"));
  EXPECT_EQ(true, filter.find("night"));
  
  EXPECT_EQ(true, filter.find("helloo"));
  EXPECT_EQ(true, filter.find("applle"));
  EXPECT_EQ(true, filter.find("moning"));
  EXPECT_EQ(true, filter.find("nait"));
}
