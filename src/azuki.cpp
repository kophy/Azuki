#include <iostream>
#include "azuki.h"
#include "machine.h"
#include "regex.h"
#include "utility.h"

namespace azuki {

Machine CreateMachine(const std::string &e) {
  bool match_begin = StartsWith(e, CARET);
  bool match_end = EndsWith(e, DOLLAR);

  // Extract positional anchors.
  int begin = match_begin? 1 : 0;
  int end = match_end? e.size() - 1 : e.size();
  std::string input = e.substr(begin, end);

  RegexpPtr r = ParseRegexp(input);
  Program p = CompileRegex(r);

  Machine m(p);
  m.SetMatchBegin(match_begin);
  m.SetMatchEnd(match_end);

  return m;
}

bool RegexSearch(const Machine &m, const std::string &s) {
  return m.Run(s).match;
}

};  // namespace azuki
