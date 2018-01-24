#include <iostream>
#include "azuki.h"
#include "machine.h"
#include "regex.h"
#include "utility.h"

namespace azuki {

Machine CreateMachine(const std::string &e) {
  bool match_begin = StartsWith(e, CARET);
  bool match_end = EndsWith(e, DOLLAR);

  // Remove positional anchors if they exist.
  int begin = match_begin ? 1 : 0;
  int end = match_end ? e.size() - 1 : e.size();
  std::string input = e.substr(begin, end);

  // Create machine with program and positonal match flags.
  RegexpPtr rp = ParseRegexp(input);
  Program program = CompileRegex(rp);

  Machine m(program);
  m.SetMatchBegin(match_begin).SetMatchEnd(match_end);
  return m;
}

bool RegexSearch(const Machine &m, const std::string &s) {
  auto status = m.Run(s);
  return status.match;
}

bool RegexSearch(const Machine &m, const std::string &s,
                 std::vector<std::string> &v) {
  auto status = m.Run(s);
  if (!status.match) return false;
  for (int i = 0; i < status.saved.size(); i += 2)
    v.push_back(std::string(status.saved[i], status.saved[i + 1]));
  return true;
}

};  // namespace azuki
