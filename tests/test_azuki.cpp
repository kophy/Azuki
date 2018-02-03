#include <iostream>
#include "azuki.h"
#include "gtest/gtest.h"

namespace Azuki {

TEST(AzukiTest, SimpleNoAnchor) {
  Machine m = CreateMachine("a+b");
  MatchStatus ms1, ms2;
  EXPECT_TRUE(RegexSearch(m, "cabd", ms1));
  EXPECT_EQ(ms1.begin, 1);
  EXPECT_EQ(ms1.end, 3);
  EXPECT_TRUE(RegexSearch(m, "aabcab", ms2));
  EXPECT_EQ(ms2.begin, 0);
  EXPECT_EQ(ms2.end, 3);
  EXPECT_FALSE(RegexSearch(m, "b"));
  EXPECT_FALSE(RegexSearch(m, "cbaa"));
}

TEST(AzukiTest, ComplexNoAnchor) {
  Machine m = CreateMachine("(ab)+");
  MatchStatus ms1, ms2;
  EXPECT_TRUE(RegexSearch(m, "cabd", ms1));
  EXPECT_EQ(ms1.begin, 1);
  EXPECT_EQ(ms1.end, 3);
  EXPECT_TRUE(RegexSearch(m, "cababad", ms2));
  EXPECT_EQ(ms2.begin, 1);
  EXPECT_EQ(ms2.end, 5);
  EXPECT_FALSE(RegexSearch(m, "ba"));
  EXPECT_FALSE(RegexSearch(m, "a b"));
}

TEST(AzukiTest, SimpleBeginAnchor) {
  Machine m = CreateMachine("^a+b");
  MatchStatus ms1, ms2;
  EXPECT_TRUE(RegexSearch(m, "ab", ms1));
  EXPECT_EQ(ms1.begin, 0);
  EXPECT_EQ(ms1.end, 2);
  EXPECT_TRUE(RegexSearch(m, "aaabcc", ms2));
  EXPECT_EQ(ms2.begin, 0);
  EXPECT_EQ(ms2.end, 4);
  EXPECT_FALSE(RegexSearch(m, "cab"));
  EXPECT_FALSE(RegexSearch(m, "aa"));
}

TEST(AzukiTest, SimpleEndAnchor) {
  Machine m = CreateMachine("a+b$");
  MatchStatus ms1, ms2;
  EXPECT_TRUE(RegexSearch(m, "ab", ms1));
  EXPECT_EQ(ms1.begin, 0);
  EXPECT_EQ(ms1.end, 2);
  EXPECT_TRUE(RegexSearch(m, "caab", ms2));
  EXPECT_EQ(ms2.begin, 1);
  EXPECT_EQ(ms2.end, 4);
  EXPECT_FALSE(RegexSearch(m, "abc"));
  EXPECT_FALSE(RegexSearch(m, "aa"));
}

TEST(AzukiTest, SimpleIterator) {
  Machine m = CreateMachine("(ab)+");
  MatchStatus ms;
  string s = "dabcccababd";
  EXPECT_TRUE(RegexSearch(m, s, ms));
  EXPECT_EQ(ms.begin, 1);
  EXPECT_EQ(ms.end, 3);
  EXPECT_TRUE(RegexSearch(m, s, ms));
  EXPECT_EQ(ms.begin, 6);
  EXPECT_EQ(ms.end, 10);
  EXPECT_FALSE(RegexSearch(m, s, ms));
}

TEST(AzukiTest, SimpleCapture) {
  Machine m = CreateMachine("^(ab)+c(ef)$");
  MatchStatus ms;
  std::vector<std::string> v;
  EXPECT_TRUE(RegexSearch(m, "ababcef", ms));
  EXPECT_EQ(ms.captured.size(), 2);
  EXPECT_EQ(ms.captured[0], "ab");
  EXPECT_EQ(ms.captured[1], "ef");
}

TEST(AzukiTest, GreedyCapture) {
  Machine m = CreateMachine("(ab+)");
  MatchStatus ms;
  std::vector<std::string> v;
  EXPECT_TRUE(RegexSearch(m, "abbb", ms));
  EXPECT_EQ(ms.captured.size(), 1);
  EXPECT_EQ(ms.captured[0], "abbb");
}

TEST(AzukiTest, SimpleReplace) {
  Machine m = CreateMachine("a+b");
  string s1 = "caabdabe";
  EXPECT_EQ(RegexReplace(m, s1, "ef"), "cefdabe");
  EXPECT_EQ(RegexReplace(m, s1, "ef", true), "cefdefe");

  string s2 = "alice";
  EXPECT_EQ(RegexReplace(m, s2, "ef"), "alice");
}

};  // namespace Azuki
