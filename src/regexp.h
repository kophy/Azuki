#ifndef __AZUKI_REGEXP__
#define __AZUKI_REGEXP__

#include "common.h"

namespace Azuki {

// Regexp types.
enum RegexpType {
  ALT,
  CAT,
  CLASS,
  CURLY,
  DOT,
  LIT,
  PAREN,
  PLUS,
  QUEST,
  STAR,
  SQUARE
};

// A Regexp struct is like a binary tree node in the tree structure built from
// raw regular expression.
// Don't use default constructor as it may lead to uninitialized fields. Use
// convenience functions(CreateAltRegexp, etc) below or ParseRegexp instead.
struct Regexp {
  // Required field(s).
  RegexpType type;

  // Optional fields (depend on Regexp type).
  char c;
  shared_ptr<Regexp> left;
  shared_ptr<Regexp> right;
  char low_ch, high_ch;       // Lower and upper bounds of character. (SQUARE)
  int low_times, high_times;  // Lower and upper bounds of character. (CURLY)
};

typedef shared_ptr<Regexp> RegexpPtr;

// Convenience functions to create different Regexps.
// Example:
//    RegexpPtr rp = CreateLitRegexp('a');
RegexpPtr CreateAltRegexp(RegexpPtr left, RegexpPtr right);
RegexpPtr CreateCatRegexp(RegexpPtr left, RegexpPtr right);
RegexpPtr CreateClassRegexp(char c);
RegexpPtr CreateCurlyRegexp(RegexpPtr left, int low_times, int high_times);
RegexpPtr CreateDotRegexp();
RegexpPtr CreateLitRegexp(char c);
RegexpPtr CreateParenRegexp(RegexpPtr left);
RegexpPtr CreatePlusRegexp(RegexpPtr left);
RegexpPtr CreateQuestRegexp(RegexpPtr left);
RegexpPtr CreateStarRegexp(RegexpPtr left);
RegexpPtr CreateSquareRegexp(char low_ch, char high_ch);

// Check the equality of two Regexp.
bool operator==(RegexpPtr rp1, RegexpPtr rp2);

// Build Regexp representation from raw regular expression.
// Example:
//    RegexpPtr rp = ParseRegexp("a+b");
RegexpPtr ParseRegexp(const std::string &s);

// Print the Regexp (for debug use).
void PrintRegexp(RegexpPtr rp);

// Check whether the Regexp is valid.
bool IsValidRegexp(RegexpPtr rp);

};  // namespace Azuki

#endif  // __AZUKI_REGEXP__
