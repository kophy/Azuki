#include "gtest/gtest.h"
#include "utility.h"

namespace azuki {

TEST(UtilityTest, StartsWith) {
  EXPECT_TRUE(StartsWith("abc", "a"));
  EXPECT_TRUE(StartsWith("abc", "ab"));
  EXPECT_FALSE(StartsWith("abc", "b"));
  EXPECT_FALSE(StartsWith("abc", "abcd"));
}

TEST(UtilityTest, EndsWith) {
  EXPECT_TRUE(EndsWith("abc", "c"));
  EXPECT_TRUE(EndsWith("abc", "bc"));
  EXPECT_FALSE(EndsWith("abc", "a"));
  EXPECT_FALSE(EndsWith("abc", "dabc"));
}

};  // namespace azuki
