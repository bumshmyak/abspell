#include <gtest/gtest.h>
#include <bloom.h>

TEST(bloom_filter_Test,StoreStrings) {
  bloom_filter filter(1024);

  filter.add("hello");
  filter.add("apple");
  filter.add("morning");
  filter.add("night");

  EXPECT_TRUE(filter.find("hello"));
  EXPECT_TRUE(filter.find("apple"));
  EXPECT_TRUE(filter.find("morning"));
  EXPECT_TRUE(filter.find("night"));
  
  EXPECT_FALSE(filter.find("helloo"));
  EXPECT_FALSE(filter.find("applle"));
  EXPECT_FALSE(filter.find("moning"));
  EXPECT_FALSE(filter.find("nait"));
}
