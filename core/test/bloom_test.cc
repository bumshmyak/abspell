
#include <gtest/gtest.h>
#include <bloom.h>

TEST(bloom_filter_Test,StoreStrings) {
  bloom_filter filter(1024);

  filter.add("hello");
  filter.add("apple");
  filter.add("morning");
  filter.add("night");

  EXPECT_EQ(filter.find("hello"), true);
  EXPECT_EQ(filter.find("apple"), true);
  EXPECT_EQ(filter.find("morning"), true);
  EXPECT_EQ(filter.find("night"), true);
  
  EXPECT_EQ(filter.find("helloo"), false);
  EXPECT_EQ(filter.find("applle"), false);
  EXPECT_EQ(filter.find("moning"), false);
  EXPECT_EQ(filter.find("nait"), false);
}
