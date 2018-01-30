#ifndef __AZUKI_REGEX_NODE__
#define __AZUKI_REGEX_NODE__

#include "common.h"

namespace Azuki {

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
// A Regexp struct must contain a valid type field, while whether other fields
// are valid depends on the Regexp type.
struct Regexp {
  // Required field(s).
  RegexpType type;

  // Optional fields.
  char c;
  shared_ptr<Regexp> left;
  shared_ptr<Regexp> right;
  char low_ch, high_ch;       // (SQUARE)
  int low_times, high_times;  // (CURLY)
};

typedef shared_ptr<Regexp> RegexpPtr;

// Convenience functions to create different Regexps.
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
RegexpPtr ParseRegexp(const std::string &s);

// Print the Regexp (for debug use).
void PrintRegexp(RegexpPtr rp);

// Check whether the Regexp is valid.
bool IsValidRegexp(RegexpPtr rp) noexcept;

};  // namespace Azuki

#endif  // __AZUKI_REGEX_NODE__
