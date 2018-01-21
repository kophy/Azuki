#include <iostream>
#include "gtest/gtest.h"
#include "instruction.h"
#include "machine.h"
#include "regexp.h"

namespace azuki {

Machine CreateMachineFromRegexp(std::shared_ptr<Regexp> r) {
  Program program = CompileRegex(r);
  return Machine(program);
}

TEST(FullTest, ComplexCase1) {
  std::shared_ptr<Regexp> r = ParseRegexp("ab?c+");
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("ac").match);
  EXPECT_TRUE(m.Run("abcc").match);
  EXPECT_FALSE(m.Run("ab").match);
  EXPECT_FALSE(m.Run("abbc").match);

#ifdef DEBUG
  PrintRegex(r1);
#endif
}

TEST(FullTest, ComplexCase2) {
  std::shared_ptr<Regexp> r = ParseRegexp("ab*|c+d");
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("a").match);
  EXPECT_TRUE(m.Run("ccd").match);
  EXPECT_FALSE(m.Run("b").match);
  EXPECT_FALSE(m.Run("cc").match);

#ifdef DEBUG
  PrintRegex(r1);
#endif
}

};  // namespace azuki
