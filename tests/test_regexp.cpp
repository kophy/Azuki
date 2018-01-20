#include "gtest/gtest.h"
#include "regexp.h"

namespace azuki {

bool IsEqualRegexp(RegexpPtr rp1, RegexpPtr rp2) {

}

TEST(RegexpTest, Foo) {
  RegexpPtr rp1 = ParseRegexp("abcd");
  RegexpPtr rp2 = ParseRegexp("ab|cd|ef");
  EXPECT_EQ(1, 1);
}

};  // namespace azuki
