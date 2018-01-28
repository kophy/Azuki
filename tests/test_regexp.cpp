#include "gtest/gtest.h"
#include "regexp.h"

namespace Azuki {

namespace {

bool IsEqualRegexp(RegexpPtr rp1, RegexpPtr rp2) {
  if (rp1->type != rp2->type) return false;
  switch (rp1->type) {
    case ALT:
    case CAT:
      return IsEqualRegexp(rp1->left, rp2->left) &&
             IsEqualRegexp(rp1->right, rp2->right);
    case CLASS:
      return rp1->c == rp2->c;
    case DOT:
      return true;
    case PAREN:
    case PLUS:
    case QUEST:
    case STAR:
      return IsEqualRegexp(rp1->left, rp2->left);
    case LIT:
      return rp1->c == rp2->c;
    case SQUARE:
      return rp1->low == rp2->low && rp1->high == rp2->high;
    default:
      return false;
  }
  return false;
}

};  // namespace

TEST(RegexpTest, SimpleChar) {
  RegexpPtr r1 = ParseRegexp("a");
  RegexpPtr r2 = CreateLitRegexp('a');
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexpTest, SimpleCat) {
  RegexpPtr r1 = ParseRegexp("ab");
  RegexpPtr r2 = CreateCatRegexp(CreateLitRegexp('a'), CreateLitRegexp('b'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexpTest, SimpleDot) {
  RegexpPtr r1 = ParseRegexp(".b");
  RegexpPtr r2 = CreateCatRegexp(CreateDotRegexp(), CreateLitRegexp('b'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexTest, SimpleAlt) {
  RegexpPtr r1 = ParseRegexp("a|b");
  RegexpPtr r2 = CreateAltRegexp(CreateLitRegexp('a'), CreateLitRegexp('b'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexTest, SimplePlus) {
  RegexpPtr r1 = ParseRegexp("a+");
  RegexpPtr r2 = CreatePlusRegexp(CreateLitRegexp('a'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexTest, SimpleQuest) {
  RegexpPtr r1 = ParseRegexp("a?");
  RegexpPtr r2 = CreateQuestRegexp(CreateLitRegexp('a'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexTest, SimpleStar) {
  RegexpPtr r1 = ParseRegexp("a*");
  RegexpPtr r2 = CreateStarRegexp(CreateLitRegexp('a'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexTest, SimpleParen) {
  RegexpPtr r1 = ParseRegexp("(a)");
  RegexpPtr r2 = CreateParenRegexp(CreateLitRegexp('a'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexTest, SimpleEscape1) {
  RegexpPtr r1 = ParseRegexp("\\w+");
  RegexpPtr r2 = CreatePlusRegexp(CreateClassRegexp('w'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexTest, SimpleEscape2) {
  RegexpPtr r1 = ParseRegexp("\\?");
  RegexpPtr r2 = CreateLitRegexp('?');
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

TEST(RegexTest, SimpleSquare) {
  RegexpPtr r1 = ParseRegexp("[a-c]+");
  RegexpPtr r2 = CreatePlusRegexp(CreateSquareRegexp('a', 'c'));
  EXPECT_TRUE(IsEqualRegexp(r1, r2));
#ifdef DEBUG
  PrintRegexp(r1);
#endif
}

};  // namespace Azuki
