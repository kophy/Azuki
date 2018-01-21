#include "gtest/gtest.h"
#include "regexp.h"

namespace azuki {

bool IsEqualRegexp(RegexpPtr rp1, RegexpPtr rp2) {
  if (rp1->type != rp2->type) return false;
  switch (rp1->type) {
    case ALT:
    case CAT:
      return IsEqualRegexp(rp1->left, rp2->left) &&
             IsEqualRegexp(rp1->right, rp2->right);
    case DOT:
      return true;
    case PLUS:
    case QUEST:
    case STAR:
      return IsEqualRegexp(rp1->left, rp2->left);
    case LIT:
      return rp1->c == rp2->c;
    default:
      return false;
  }
  return false;
}

TEST(RegexpTest, SimpleChar) {
  std::shared_ptr<Regexp> r1 = ParseRegexp("a");
  std::shared_ptr<Regexp> r2(new Regexp(LIT, 'a'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintProgram(r1);
#endif
}

TEST(RegexpTest, SimpleCat) {
  std::shared_ptr<Regexp> r1 = ParseRegexp("ab");
  std::shared_ptr<Regexp> r2(new Regexp(CAT));
  r2->left.reset(new Regexp(LIT, 'a'));
  r2->right.reset(new Regexp(LIT, 'b'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintProgram(r1);
#endif
}

TEST(RegexpTest, SimpleDot) {
  std::shared_ptr<Regexp> r1 = ParseRegexp(".b");
  std::shared_ptr<Regexp> r2(new Regexp(CAT));
  r2->left.reset(new Regexp(DOT));
  r2->right.reset(new Regexp(LIT, 'b'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintProgram(r1);
#endif
}

TEST(RegexTest, SimpleAlt) {
  std::shared_ptr<Regexp> r1 = ParseRegexp("a|b");
  std::shared_ptr<Regexp> r2(new Regexp(ALT));
  r2->left.reset(new Regexp(LIT, 'a'));
  r2->right.reset(new Regexp(LIT, 'b'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintProgram(r1);
#endif
}

TEST(RegexTest, SimplePlus) {
  std::shared_ptr<Regexp> r1 = ParseRegexp("a+");
  std::shared_ptr<Regexp> r2(new Regexp(PLUS));
  r2->left.reset(new Regexp(LIT, 'a'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintProgram(r1);
#endif
}

TEST(RegexTest, SimpleQuest) {
  std::shared_ptr<Regexp> r1 = ParseRegexp("a?");
  std::shared_ptr<Regexp> r2(new Regexp(QUEST));
  r2->left.reset(new Regexp(LIT, 'a'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintProgram(r1);
#endif
}

TEST(RegexTest, SimpleStar) {
  std::shared_ptr<Regexp> r1 = ParseRegexp("a*");
  std::shared_ptr<Regexp> r2(new Regexp(STAR));
  r2->left.reset(new Regexp(LIT, 'a'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintProgram(r1);
#endif
}

};  // namespace azuki
