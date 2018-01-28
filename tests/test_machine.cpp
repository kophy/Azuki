#include "gtest/gtest.h"
#include "machine.h"

namespace Azuki {

namespace {

Machine CreateMachineFromRegexp(RegexpPtr r) {
  Program program = CompileRegexp(r);
  Machine m(program);
  m.SetMatchBegin(true);
  return m;
}

};  // namespace

TEST(MachineTest, SingleChar) {
  // match "a"
  RegexpPtr rp = CreateLitRegexp('a');
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("a").success);
  EXPECT_TRUE(m.Run("abc").success);
  EXPECT_FALSE(m.Run("b").success);
  EXPECT_FALSE(m.Run("bac").success);
}

TEST(MachineTest, SimpleCat) {
  // match "ab"
  RegexpPtr rp = CreateCatRegexp(CreateLitRegexp('a'), CreateLitRegexp('b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("ab").success);
  EXPECT_TRUE(m.Run("abc").success);
  EXPECT_FALSE(m.Run("a").success);
  EXPECT_FALSE(m.Run("bab").success);
}

TEST(MachineTest, SimpleDot) {
  // match ".b"
  RegexpPtr rp = CreateCatRegexp(CreateDotRegexp(), CreateLitRegexp('b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("ab").success);
  EXPECT_TRUE(m.Run("cba").success);
  EXPECT_FALSE(m.Run("b").success);
  EXPECT_FALSE(m.Run("cab").success);
}

TEST(MachineTest, SimpleSplit) {
  // match "a|b"
  RegexpPtr rp = CreateAltRegexp(CreateLitRegexp('a'), CreateLitRegexp('b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("a").success);
  EXPECT_TRUE(m.Run("b").success);
  EXPECT_FALSE(m.Run("c").success);
}

TEST(MachineTest, SimplePlus) {
  // match "a+"
  RegexpPtr rp = CreatePlusRegexp(CreateLitRegexp('a'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("a").success);
  EXPECT_TRUE(m.Run("aa").success);
  EXPECT_FALSE(m.Run("b").success);
}

TEST(MachineTest, SimpleQuest) {
  // match "a?b"
  auto left = CreateQuestRegexp(CreateLitRegexp('a'));
  RegexpPtr rp = CreateCatRegexp(left, CreateLitRegexp('b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("b").success);
  EXPECT_TRUE(m.Run("ab").success);
  EXPECT_FALSE(m.Run("cab").success);
}

TEST(MachineTest, SimpleStar) {
  // match "a*b"
  auto left = CreateStarRegexp(CreateLitRegexp('a'));
  RegexpPtr rp = CreateCatRegexp(left, CreateLitRegexp('b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("b").success);
  EXPECT_TRUE(m.Run("ab").success);
  EXPECT_TRUE(m.Run("aaab").success);
  EXPECT_FALSE(m.Run("cab").success);
}

TEST(MachineTest, SimpleParen) {
  // match "(a+)"
  RegexpPtr rp = CreateParenRegexp(CreatePlusRegexp(CreateLitRegexp('a')));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("a").success);
  EXPECT_TRUE(m.Run("aa").success);
  EXPECT_FALSE(m.Run("b").success);
  EXPECT_FALSE(m.Run("baac").success);
}

TEST(MachineTest, SimpleWordClass) {
  // match "\w+"
  RegexpPtr rp = CreatePlusRegexp(CreateClassRegexp('w'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("ab_12").success);
  EXPECT_TRUE(m.Run("12_ab").success);
  EXPECT_FALSE(m.Run("\ta1").success);
  EXPECT_FALSE(m.Run("?a1").success);
}

TEST(MachineTest, SimpleDigitClass) {
  // match "\d+"
  RegexpPtr rp = CreatePlusRegexp(CreateClassRegexp('d'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("1234").success);
  EXPECT_TRUE(m.Run("023").success);
  EXPECT_FALSE(m.Run(" 12").success);
  EXPECT_FALSE(m.Run("a1").success);
}

TEST(MachineTest, SimpleSpaceClass) {
  // match "\s+"
  RegexpPtr rp = CreatePlusRegexp(CreateClassRegexp('s'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("  a1").success);
  EXPECT_TRUE(m.Run("\t\ra1").success);
  EXPECT_FALSE(m.Run("a1").success);
  EXPECT_FALSE(m.Run("?a1").success);
}

};  // namespace Azuki
