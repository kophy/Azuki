#include <iostream>
#include "azuki.h"
#include "gtest/gtest.h"

namespace Azuki {

TEST(AzukiTest, SimpleNoAnchor) {
  Machine m = CreateMachine("a+b");
  MatchResult result1, result2;
  EXPECT_TRUE(RegexSearch(m, "cabd", result1));
  EXPECT_EQ(result1.begin, 1);
  EXPECT_EQ(result1.end, 3);
  EXPECT_TRUE(RegexSearch(m, "aabcab", result2));
  EXPECT_EQ(result2.begin, 0);
  EXPECT_EQ(result2.end, 3);
  EXPECT_FALSE(RegexSearch(m, "b"));
  EXPECT_FALSE(RegexSearch(m, "cbaa"));
}

TEST(AzukiTest, ComplexNoAnchor) {
  Machine m = CreateMachine("(ab)+");
  MatchResult result1, result2;
  EXPECT_TRUE(RegexSearch(m, "cabd", result1));
  EXPECT_EQ(result1.begin, 1);
  EXPECT_EQ(result1.end, 3);
  EXPECT_TRUE(RegexSearch(m, "cababad", result2));
  EXPECT_EQ(result2.begin, 1);
  EXPECT_EQ(result2.end, 5);
  EXPECT_FALSE(RegexSearch(m, "ba"));
  EXPECT_FALSE(RegexSearch(m, "a b"));
}

TEST(AzukiTest, SimpleBeginAnchor) {
  Machine m = CreateMachine("^a+b");
  MatchResult result1, result2;
  EXPECT_TRUE(RegexSearch(m, "ab", result1));
  EXPECT_EQ(result1.begin, 0);
  EXPECT_EQ(result1.end, 2);
  EXPECT_TRUE(RegexSearch(m, "aaabcc", result2));
  EXPECT_EQ(result2.begin, 0);
  EXPECT_EQ(result2.end, 4);
  EXPECT_FALSE(RegexSearch(m, "cab"));
  EXPECT_FALSE(RegexSearch(m, "aa"));
}

TEST(AzukiTest, SimpleEndAnchor) {
  Machine m = CreateMachine("a+b$");
  MatchResult result1, result2;
  EXPECT_TRUE(RegexSearch(m, "ab", result1));
  EXPECT_EQ(result1.begin, 0);
  EXPECT_EQ(result1.end, 2);
  EXPECT_TRUE(RegexSearch(m, "caab", result2));
  EXPECT_EQ(result2.begin, 1);
  EXPECT_EQ(result2.end, 4);
  EXPECT_FALSE(RegexSearch(m, "abc"));
  EXPECT_FALSE(RegexSearch(m, "aa"));
}

TEST(AzukiTest, SimpleIterator) {
  Machine m = CreateMachine("(ab)+");
  MatchResult result;
  string s = "dabcccababd";
  EXPECT_TRUE(RegexSearch(m, s, result));
  EXPECT_EQ(result.begin, 1);
  EXPECT_EQ(result.end, 3);
  EXPECT_TRUE(RegexSearch(m, s, result));
  EXPECT_EQ(result.begin, 6);
  EXPECT_EQ(result.end, 10);
  EXPECT_FALSE(RegexSearch(m, s, result));
}

TEST(AzukiTest, SimpleCapture) {
  Machine m = CreateMachine("^(ab)+c(ef)$");
  MatchResult result;
  std::vector<std::string> v;
  EXPECT_TRUE(RegexSearch(m, "ababcef", result));
  EXPECT_EQ(result.capture.size(), 2);
  EXPECT_EQ(result.capture[0], "ab");
  EXPECT_EQ(result.capture[1], "ef");
}

TEST(AzukiTest, GreedyCapture) {
  Machine m = CreateMachine("(ab+)");
  MatchResult result;
  std::vector<std::string> v;
  EXPECT_TRUE(RegexSearch(m, "abbb", result));
  EXPECT_EQ(result.capture.size(), 1);
  EXPECT_EQ(result.capture[0], "abbb");
}

TEST(AzukiTest, SimpleReplace) {
  Machine m = CreateMachine("a+b");
  string s1 = "caabdabe";
  EXPECT_EQ(RegexReplace(m, s1, "ef"), "cefdabe");
  EXPECT_EQ(RegexReplace(m, s1, "ef", true), "cefdefe");

  string s2 = "alice";
  EXPECT_EQ(RegexReplace(m, s2, "ef"), "alice");
}

TEST(AzukiTest, ReferenceReplace) {
  Machine m = CreateMachine("(a+)b");
  string s = "caabdabe";
  EXPECT_EQ(RegexReplace(m, s, "$0ff"), "caaffdabe");
  EXPECT_EQ(RegexReplace(m, s, "$0ff", true), "caaffdaffe");
}

};  // namespace Azuki
