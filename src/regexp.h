#ifndef __AZUKI_REGEX_NODE__
#define __AZUKI_REGEX_NODE__

#include <memory>

namespace azuki {

enum RegexType {
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
  RegexType type;
  char c;
  std::shared_ptr<Regexp> left, right;

  Regexp(RegexType type_, char c_ = ' ') : type(type_), c(c_) {}
} RegexNode;

};  // namespace azuki

#endif
