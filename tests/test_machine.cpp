#include "gtest/gtest.h"
#include "machine.h"

namespace azuki {

TEST(MachineTest, SingleCharInstruction) {
  std::shared_ptr<Regexp> r(new Regexp(LIT, 'a'));
  Program program = CompileRegex(r);
  Machine machine(program);
  EXPECT_TRUE(machine.Execute("a"));
  EXPECT_TRUE(machine.Execute("bca"));
  EXPECT_FALSE(machine.Execute("b"));
  EXPECT_FALSE(machine.Execute("bbb"));
}

TEST(InstructionTest, SimpleSplitInstruction) {
  std::shared_ptr<Regexp> r(new Regexp(ALT));
  r->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Program program = CompileRegex(r);
  Machine machine(program);
  EXPECT_TRUE(machine.Execute("a"));
  EXPECT_TRUE(machine.Execute("b"));
  EXPECT_FALSE(machine.Execute("c"));
}

};  // namespace azuki
