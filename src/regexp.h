#ifndef __AZUKI_REGEX_NODE__
#define __AZUKI_REGEX_NODE__

#include <memory>
#include <string>

namespace azuki {

enum RegexpType { ALT, CAT, DOT, LIT, PAREN, PLUS, QUEST, STAR };

// A Regexp struct is like a binary tree node in the tree structure built from
// raw regular expression.
// A Regexp struct must contain a valid type field, while whether other fields
// are invalid depends on the Regexp type.
struct Regexp {
  RegexpType type;
  char c;
  std::shared_ptr<Regexp> left;
  std::shared_ptr<Regexp> right;

  Regexp(RegexpType type, char c = ' ') : type(type), c(c) {}
};

typedef std::shared_ptr<Regexp> RegexpPtr;

// Convenience functions to create different regexps.
RegexpPtr CreateALTRegexp(RegexpPtr left, RegexpPtr right);
RegexpPtr CreateCATRegexp(RegexpPtr left, RegexpPtr right);
RegexpPtr CreateDOTRegexp();
RegexpPtr CreateLITRegexp(char c);
RegexpPtr CreatePARENRegexp(RegexpPtr left);
RegexpPtr CreatePLUSRegexp(RegexpPtr left);
RegexpPtr CreateQUESTRegexp(RegexpPtr left);
RegexpPtr CreateSTARRegexp(RegexpPtr left);

// Build Regexp representation from raw regular expression s.
RegexpPtr ParseRegexp(const std::string &s);

// Print the regexp (for debug use).
void PrintRegexp(RegexpPtr rp);

// Check whether the Regexp is valid.
bool IsValidRegexp(RegexpPtr rp);

};  // namespace azuki

#endif
