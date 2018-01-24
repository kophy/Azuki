#include <iostream>
#include "gtest/gtest.h"
#include "azuki.h"

namespace azuki {

TEST(AzukiTest, SimpleBeginAnchor) {
  Machine m = CreateMachine("^a+b");
  EXPECT_TRUE(RegexSearch(m, "ab"));
  EXPECT_TRUE(RegexSearch(m, "aaab"));
  EXPECT_FALSE(RegexSearch(m, "cab"));
  EXPECT_FALSE(RegexSearch(m, "aa"));
}

TEST(AzukiTest, SimpleEndAnchor) {
  Machine m = CreateMachine("a+b$");
  EXPECT_TRUE(RegexSearch(m, "ab"));
  EXPECT_TRUE(RegexSearch(m, "caab"));
  EXPECT_FALSE(RegexSearch(m, "abc"));
  EXPECT_FALSE(RegexSearch(m, "aa"));
}

TEST(AzukiTest, SimpleBothAnchor) {
  Machine m = CreateMachine("a+b$");
  EXPECT_TRUE(RegexSearch(m, "ab"));
  EXPECT_TRUE(RegexSearch(m, "caab"));
  EXPECT_FALSE(RegexSearch(m, "abc"));
  EXPECT_FALSE(RegexSearch(m, "aa"));
}

TEST(AzukiTest, SimpleCapture) {
  Machine m = CreateMachine("^(ab)+c(ef)$");
  std::vector<std::string> v;
  EXPECT_TRUE(RegexSearch(m, "ababcef", v));
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], "ab");
  EXPECT_EQ(v[1], "ef");
}

TEST(AzukiTest, GreedyCapture) {
  Machine m = CreateMachine("(ab+)");
  std::vector<std::string> v;
  EXPECT_TRUE(RegexSearch(m, "abbb", v));
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], "abbb");
}

};  // namespace azuki
