#include <iostream>
#include "gtest/gtest.h"
#include "instruction.h"

namespace azuki {

TEST(InstructionTest, InvalidInstruction) {
  Instruction instr;
  EXPECT_THROW(instr.str(), std::exception);
}

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

TEST(InstructionTest, SimpleDot) {
  std::shared_ptr<Regexp> r(new Regexp(CAT));
  r->left.reset(new Regexp(DOT));
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

TEST(InstructionTest, SimplePlus) {
  std::shared_ptr<Regexp> r(new Regexp(PLUS));
  r->left.reset(new Regexp(LIT, 'a'));
  Program program = CompileRegex(r);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleQuest) {
  std::shared_ptr<Regexp> r(new Regexp(QUEST));
  r->left.reset(new Regexp(LIT, 'a'));
  Program program = CompileRegex(r);
  EXPECT_EQ(program.size(), 3);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleStar) {
  std::shared_ptr<Regexp> r(new Regexp(STAR));
  r->left.reset(new Regexp(LIT, 'a'));
  Program program = CompileRegex(r);
  EXPECT_EQ(program.size(), 4);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

};  // namespace azuki
