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

};  // namespace azuki
