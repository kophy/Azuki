#include "gtest/gtest.h"
#include "machine.h"

namespace azuki {

TEST(MachineTest, SingleChar) {
  std::shared_ptr<Regexp> r(new Regexp(LIT, 'a'));
  Program program = CompileRegex(r);
  Machine machine(program);
  EXPECT_TRUE(machine.Run("a"));
  EXPECT_TRUE(machine.Run("bca"));
  EXPECT_FALSE(machine.Run("b"));
  EXPECT_FALSE(machine.Run("bbb"));
}

TEST(MachineTest, SimpleCat) {
  std::shared_ptr<Regexp> r(new Regexp(CAT));
  r->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Program program = CompileRegex(r);
  Machine machine(program);
  EXPECT_TRUE(machine.Run("ab"));
  EXPECT_TRUE(machine.Run("cab"));
  EXPECT_FALSE(machine.Run("a"));
  EXPECT_FALSE(machine.Run("bc"));
}

TEST(InstructionTest, SimpleSplit) {
  std::shared_ptr<Regexp> r(new Regexp(ALT));
  r->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Program program = CompileRegex(r);
  Machine machine(program);
  EXPECT_TRUE(machine.Run("a"));
  EXPECT_TRUE(machine.Run("b"));
  EXPECT_FALSE(machine.Run("c"));
}

};  // namespace azuki
