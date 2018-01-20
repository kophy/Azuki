#ifndef __AZUKI_REGEX_NODE__
#define __AZUKI_REGEX_NODE__

#include <memory>
#include <string>

namespace azuki {

enum RegexpType {
  ALT,
  CAT,
  LIT,
  DOT,
  PAREN,
  QUEST,
  STAR,
  PLUS
};

typedef struct Regexp {
  RegexpType type;
  char c;
  std::shared_ptr<Regexp> left, right;

  Regexp(RegexpType type, char c = ' ') : type(type), c(c) {}
} Regexp;

typedef std::shared_ptr<Regexp> RegexpPtr;

RegexpPtr ParseRegexp(const std::string &s);

};  // namespace azuki

#endif
