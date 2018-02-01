#include <iostream>
#include <sstream>
#include "azuki.h"
#include "machine.h"
#include "regex.h"
#include "utility.h"

namespace Azuki {

Machine CreateMachine(const string &e) {
  bool match_begin = StartsWith(e, CARET);
  bool match_end =
      EndsWith(e, DOLLAR) && !EndsWith(e, "\\" + string(1, DOLLAR));

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
  auto ms = m.Run(s);
  return ms.success;
}

bool RegexSearch(const Machine &m, const string &s, vector<string> &v) {
  auto ms = m.Run(s);
  if (!ms.success) return false;
  for (unsigned int i = 0; i < ms.saved.size(); i += 2)
    v.push_back(string(ms.saved[i], ms.saved[i + 1]));
  return ms.success;
}

bool RegexSearch(const Machine &m, const string &s, MatchStatus &ms) {
  unsigned int offset = ms.end_idx;
  auto temp = m.Run(s.substr(offset));
  if (!temp.success) return false;
  temp.begin_idx += offset;
  temp.end_idx += offset;
  ms = temp;
  return ms.success;
}

bool RegexSearch(const Machine &m, const string &s, MatchStatus &ms,
                 vector<string> &v) {
  unsigned int offset = ms.end_idx;
  auto temp = m.Run(s.substr(offset));
  if (!temp.success) return false;
  temp.begin_idx += offset;
  temp.end_idx += offset;
  ms = temp;
  for (unsigned int i = 0; i < ms.saved.size(); i += 2)
    v.push_back(string(ms.saved[i], ms.saved[i + 1]));
  return true;
}

string RegexReplace(const Machine &m, const string &s, const string &new_subs,
                    bool global) {
  vector<pair<int, int>> replaced;
  auto ms = m.Run(s);
  if (ms.success)
    replaced.push_back(std::make_pair(ms.begin_idx, ms.end_idx));
  if (global) {
    while (RegexSearch(m, s, ms)) {
      replaced.push_back(std::make_pair(ms.begin_idx, ms.end_idx));
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
