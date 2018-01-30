#include <iostream>
#include "gtest/gtest.h"
#include "instruction.h"

namespace Azuki {

TEST(InstructionTest, InvalidInstruction) {
  Instruction instr(Opcode(JMP + 1));
  EXPECT_THROW(instr.str(), std::runtime_error);
}

TEST(InstructionTest, SingleChar) {
  // "a"
  RegexpPtr rp = CreateLitRegexp('a');
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 2);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleCat) {
  // "ab"
  RegexpPtr rp = CreateCatRegexp(CreateLitRegexp('a'), CreateLitRegexp('b'));
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 3);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleDot) {
  // ".b"
  RegexpPtr rp = CreateCatRegexp(CreateDotRegexp(), CreateLitRegexp('b'));
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 3);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleAlt) {
  // "a|b"
  RegexpPtr rp = CreateAltRegexp(CreateDotRegexp(), CreateLitRegexp('b'));
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 5);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimplePlus) {
  // "a+"
  RegexpPtr rp = CreatePlusRegexp(CreateLitRegexp('a'));
  Program program = CompileRegexp(rp);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleQuest) {
  // "a?"
  RegexpPtr rp = CreateQuestRegexp(CreateLitRegexp('a'));
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 3);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleStar) {
  // "a*"
  RegexpPtr rp = CreateStarRegexp(CreateLitRegexp('a'));
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 4);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleParen) {
  // "(b)"
  RegexpPtr rp = CreateParenRegexp(CreateLitRegexp('a'));
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 4);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleEscape) {
  // "\w+"
  RegexpPtr rp = CreatePlusRegexp(CreateClassRegexp('w'));
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 4);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleSquare) {
  // "[a-c]+"
  RegexpPtr rp = CreatePlusRegexp(CreateSquareRegexp('a', 'c'));
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 4);
#ifdef DEBUG
  PrintProgram(program);
#endif
}

TEST(InstructionTest, SimpleCurly) {
  // "a{3,5}"
  RegexpPtr rp = ParseRegexp("a{3,5}");
  Program program = CompileRegexp(rp);
  EXPECT_EQ(program.size(), 7);
#ifndef DEBUG
  PrintProgram(program);
#endif
}

};  // namespace Azuki
