#include <iostream>
#include "azuki.h"
#include "machine.h"
#include "regex.h"
#include "utility.h"

namespace Azuki {

Machine CreateMachine(const string &e) {
  bool match_begin = StartsWith(e, CARET);
  bool match_end = EndsWith(e, DOLLAR);

  // Remove positional anchors if they exist.
  int begin = match_begin ? 1 : 0;
  int end = match_end ? e.size() - 1 : e.size();
  std::string input = e.substr(begin, end);

  // Create machine with program and positonal match flags.
  RegexpPtr rp = ParseRegexp(input);
  Program program = CompileRegexp(rp);

  Machine m(program);
  m.SetMatchBegin(match_begin).SetMatchEnd(match_end);
  return m;
}

bool RegexSearch(const Machine &m, const string &s) {
  auto status = m.Run(s);
  return status.success;
}

bool RegexSearch(const Machine &m, const string &s, vector<string> &v) {
  auto status = m.Run(s);
  if (!status.success) return false;
  for (unsigned int i = 0; i < status.saved.size(); i += 2)
    v.push_back(string(status.saved[i], status.saved[i + 1]));
  return true;
}

};  // namespace Azuki
