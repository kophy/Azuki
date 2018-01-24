#include "gtest/gtest.h"
#include "machine.h"

namespace azuki {

Machine CreateMachineFromRegexp(std::shared_ptr<Regexp> r) {
  Program program = CompileRegex(r);
  Machine m(program);
  m.SetMatchBegin(true);
  return m;
}

TEST(MachineTest, SingleChar) {
  // match "a"
  std::shared_ptr<Regexp> r(new Regexp(LIT, 'a'));
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("a").match);
  EXPECT_TRUE(m.Run("abc").match);
  EXPECT_FALSE(m.Run("b").match);
  EXPECT_FALSE(m.Run("bac").match);
}

TEST(MachineTest, SimpleCat) {
  // match "ab"
  std::shared_ptr<Regexp> r(new Regexp(CAT));
  r->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("ab").match);
  EXPECT_TRUE(m.Run("abc").match);
  EXPECT_FALSE(m.Run("a").match);
  EXPECT_FALSE(m.Run("bab").match);
}

TEST(MachineTest, SimpleDot) {
  // match ".b"
  std::shared_ptr<Regexp> r(new Regexp(CAT));
  r->left.reset(new Regexp(DOT));
  r->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("ab").match);
  EXPECT_TRUE(m.Run("cba").match);
  EXPECT_FALSE(m.Run("b").match);
  EXPECT_FALSE(m.Run("cab").match);
}

TEST(MachineTest, SimpleSplit) {
  // match "a|b"
  std::shared_ptr<Regexp> r(new Regexp(ALT));
  r->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("a").match);
  EXPECT_TRUE(m.Run("b").match);
  EXPECT_FALSE(m.Run("c").match);
}

TEST(MachineTest, SimplePlus) {
  // match "a+"
  std::shared_ptr<Regexp> r(new Regexp(PLUS));
  r->left.reset(new Regexp(LIT, 'a'));
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("a").match);
  EXPECT_TRUE(m.Run("aa").match);
  EXPECT_FALSE(m.Run("b").match);
}

TEST(MachineTest, SimpleQuest) {
  // match "a?b"
  std::shared_ptr<Regexp> r(new Regexp(CAT));
  r->left.reset(new Regexp(QUEST));
  r->left->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("b").match);
  EXPECT_TRUE(m.Run("ab").match);
  EXPECT_FALSE(m.Run("cab").match);
}

TEST(MachineTest, SimpleStar) {
  // match "a*b"
  std::shared_ptr<Regexp> r(new Regexp(CAT));
  r->left.reset(new Regexp(STAR));
  r->left->left.reset(new Regexp(LIT, 'a'));
  r->right.reset(new Regexp(LIT, 'b'));
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("b").match);
  EXPECT_TRUE(m.Run("ab").match);
  EXPECT_TRUE(m.Run("aaab").match);
  EXPECT_FALSE(m.Run("cab").match);
}

TEST(MachineTest, SimpleParen) {
  // match "(a+)"
  std::shared_ptr<Regexp> r(new Regexp(PAREN));
  r->left.reset(new Regexp(PLUS));
  r->left->left.reset(new Regexp(LIT, 'a'));
  Machine m = CreateMachineFromRegexp(r);
  EXPECT_TRUE(m.Run("a").match);
  EXPECT_TRUE(m.Run("aa").match);
  EXPECT_FALSE(m.Run("b").match);
  EXPECT_FALSE(m.Run("baac").match);
}

};  // namespace azuki
