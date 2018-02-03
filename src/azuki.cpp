#include <iostream>
#include <sstream>
#include "azuki.h"
#include "machine.h"
#include "regex.h"
#include "utility.h"

namespace Azuki {

Machine CreateMachine(const string &e) {
  bool match_begin = StartsWith(e, '^');
  bool match_end = EndsWith(e, '$') && !EndsWith(e, "\\$");

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
  auto ms = m.Run(s, false);
  return ms.success;
}

bool RegexSearch(const Machine &m, const string &s, MatchStatus &ms,
                 bool capture) {
  unsigned int offset = ms.end;
  auto temp = m.Run(s.substr(offset), capture);
  if (!temp.success) return false;
  temp.begin += offset;
  temp.end += offset;
  ms = temp;
  return ms.success;
}

string RegexReplace(const Machine &m, const string &s, const string &new_subs,
                    bool global) {
  vector<pair<int, int>> replaced;
  auto ms = m.Run(s);
  if (ms.success) replaced.push_back(std::make_pair(ms.begin, ms.end));
  if (global) {
    while (RegexSearch(m, s, ms)) {
      replaced.push_back(std::make_pair(ms.begin, ms.end));
    }
  }
  std::stringstream ss;
  int pos = 0;
  for (auto &p : replaced) {
    ss << s.substr(pos, p.first - pos) << new_subs;
    pos = p.second;
  }
  ss << s.substr(pos);
  return ss.str();
}

};  // namespace Azuki
