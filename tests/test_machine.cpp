#include "gtest/gtest.h"
#include "machine.h"

namespace Azuki {

Machine CreateMachineFromRegexp(RegexpPtr r) {
  Program program = CompileRegexp(r);
  Machine m(program);
  m.SetMatchBegin(true);
  return m;
}

TEST(MachineTest, SingleChar) {
  // match "a"
  RegexpPtr rp(new Regexp(LIT, 'a'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("a").match);
  EXPECT_TRUE(m.Run("abc").match);
  EXPECT_FALSE(m.Run("b").match);
  EXPECT_FALSE(m.Run("bac").match);
}

TEST(MachineTest, SimpleCat) {
  // match "ab"
  RegexpPtr rp(new Regexp(CAT));
  rp->left.reset(new Regexp(LIT, 'a'));
  rp->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("ab").match);
  EXPECT_TRUE(m.Run("abc").match);
  EXPECT_FALSE(m.Run("a").match);
  EXPECT_FALSE(m.Run("bab").match);
}

TEST(MachineTest, SimpleDot) {
  // match ".b"
  RegexpPtr rp(new Regexp(CAT));
  rp->left.reset(new Regexp(DOT));
  rp->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("ab").match);
  EXPECT_TRUE(m.Run("cba").match);
  EXPECT_FALSE(m.Run("b").match);
  EXPECT_FALSE(m.Run("cab").match);
}

TEST(MachineTest, SimpleSplit) {
  // match "a|b"
  RegexpPtr rp(new Regexp(ALT));
  rp->left.reset(new Regexp(LIT, 'a'));
  rp->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("a").match);
  EXPECT_TRUE(m.Run("b").match);
  EXPECT_FALSE(m.Run("c").match);
}

TEST(MachineTest, SimplePlus) {
  // match "a+"
  RegexpPtr rp(new Regexp(PLUS));
  rp->left.reset(new Regexp(LIT, 'a'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("a").match);
  EXPECT_TRUE(m.Run("aa").match);
  EXPECT_FALSE(m.Run("b").match);
}

TEST(MachineTest, SimpleQuest) {
  // match "a?b"
  RegexpPtr rp(new Regexp(CAT));
  rp->left.reset(new Regexp(QUEST));
  rp->left->left.reset(new Regexp(LIT, 'a'));
  rp->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("b").match);
  EXPECT_TRUE(m.Run("ab").match);
  EXPECT_FALSE(m.Run("cab").match);
}

TEST(MachineTest, SimpleStar) {
  // match "a*b"
  RegexpPtr rp(new Regexp(CAT));
  rp->left.reset(new Regexp(STAR));
  rp->left->left.reset(new Regexp(LIT, 'a'));
  rp->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("b").match);
  EXPECT_TRUE(m.Run("ab").match);
  EXPECT_TRUE(m.Run("aaab").match);
  EXPECT_FALSE(m.Run("cab").match);
}

TEST(MachineTest, SimpleParen) {
  // match "(a+)"
  RegexpPtr rp(new Regexp(PAREN));
  rp->left.reset(new Regexp(PLUS));
  rp->left->left.reset(new Regexp(LIT, 'a'));
  Machine m = CreateMachineFromRegexp(rp);
  EXPECT_TRUE(m.Run("a").match);
  EXPECT_TRUE(m.Run("aa").match);
  EXPECT_FALSE(m.Run("b").match);
  EXPECT_FALSE(m.Run("baac").match);
}

};  // namespace Azuki
