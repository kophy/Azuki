#include <iostream>
#include "gtest/gtest.h"
#include "instruction.h"

namespace azuki {

TEST(InstructionTest, SingleChar) {
  std::shared_ptr<Regexp> r(new Regexp(LIT, 'a'));
  Program program = CompileRegex(r);
  EXPECT_EQ(program.size(), 2);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleCat) {
  std::shared_ptr<Regexp> r(new Regexp(CAT));
  r->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Program program = CompileRegex(r);
  EXPECT_EQ(program.size(), 3);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleAlt) {
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
