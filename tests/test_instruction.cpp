#include <iostream>
#include "gtest/gtest.h"
#include "instruction.h"

namespace azuki {

TEST(InstructionTest, SingleCharInstruction) {
  std::shared_ptr<Regexp> r(new Regexp(LIT, 'ab'));
  Program program = CompileRegex(r);
  EXPECT_EQ(program.size(), 2);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleSplitInstruction) {
  std::shared_ptr<Regexp> r(new Regexp(ALT));
  r->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Program program = CompileRegex(r);
  EXPECT_EQ(program.size(), 5);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

};  // namespace azuki
