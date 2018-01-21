#include "gtest/gtest.h"
#include "regexp.h"

namespace azuki {

bool IsEqualRegexp(RegexpPtr rp1, RegexpPtr rp2) {
  return true;
}

TEST(RegexpTest, Foo) {
  RegexpPtr rp1 = ParseRegexp("abcd");
  RegexpPtr rp2 = ParseRegexp("ab|cd");
  EXPECT_EQ(1, 1);
}

};  // namespace azuki
