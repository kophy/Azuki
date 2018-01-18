#include "gtest/gtest.h"
#include "machine.h"

namespace azuki {

TEST(MachineTest, SingleChar) {
  std::shared_ptr<Regexp> r(new Regexp(LIT, 'a'));
  Program program = CompileRegex(r);
  Machine machine(program);
  EXPECT_TRUE(machine.Run("a").match);
  EXPECT_TRUE(machine.Run("bca").match);
  EXPECT_FALSE(machine.Run("b").match);
  EXPECT_FALSE(machine.Run("bbb").match);
}

TEST(MachineTest, SimpleCat) {
  std::shared_ptr<Regexp> r(new Regexp(CAT));
  r->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Program program = CompileRegex(r);
  Machine machine(program);
  EXPECT_TRUE(machine.Run("ab").match);
  EXPECT_TRUE(machine.Run("cab").match);
  EXPECT_FALSE(machine.Run("a").match);
  EXPECT_FALSE(machine.Run("bc").match);
}

TEST(InstructionTest, SimpleSplit) {
  std::shared_ptr<Regexp> r(new Regexp(ALT));
  r->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Program program = CompileRegex(r);
  Machine machine(program);
  EXPECT_TRUE(machine.Run("a").match);
  EXPECT_TRUE(machine.Run("b").match);
  EXPECT_FALSE(machine.Run("c").match);
}

};  // namespace azuki
