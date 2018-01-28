#ifndef __AZUKI_REGEX_NODE__
#define __AZUKI_REGEX_NODE__

#include "common.h"

namespace Azuki {

enum RegexpType { ALT, CAT, CLASS, DOT, LIT, PAREN, PLUS, QUEST, STAR };

// A Regexp struct is like a binary tree node in the tree structure built from
// raw regular expression.
// A Regexp struct must contain a valid type field, while whether other fields
// are valid depends on the Regexp type.
struct Regexp {
  RegexpType type;
  char c;
  shared_ptr<Regexp> left;
  shared_ptr<Regexp> right;
};

typedef shared_ptr<Regexp> RegexpPtr;

// Convenience functions to create different Regexps.
RegexpPtr CreateAltRegexp(RegexpPtr left, RegexpPtr right);
RegexpPtr CreateCatRegexp(RegexpPtr left, RegexpPtr right);
RegexpPtr CreateClassRegexp(char c);
RegexpPtr CreateDotRegexp();
RegexpPtr CreateLitRegexp(char c);
RegexpPtr CreateParenRegexp(RegexpPtr left);
RegexpPtr CreatePlusRegexp(RegexpPtr left);
RegexpPtr CreateQuestRegexp(RegexpPtr left);
RegexpPtr CreateStarRegexp(RegexpPtr left);

// Build Regexp representation from raw regular expression.
RegexpPtr ParseRegexp(const std::string &s);

// Print the Regexp (for debug use).
void PrintRegexp(RegexpPtr rp);

// Check whether the Regexp is valid.
bool IsValidRegexp(RegexpPtr rp) noexcept;

};  // namespace Azuki

#endif  // __AZUKI_REGEX_NODE__
