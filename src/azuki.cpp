#include <sstream>
#include "azuki.h"
#include "utility.h"

namespace Azuki {

namespace {

string CreateNewSubs(const string &fmt, const vector<string> &capture) {
  std::stringstream ss;
  unsigned idx = 0;
  while (idx < fmt.size()) {
    if (fmt[idx] != '$') {
      ss << fmt[idx++];
    } else {
      ++idx;
      if (idx >= fmt.size())
        throw std::runtime_error("Unexpected format string.");
      if (fmt[idx] == '$') {
        ss << '$';
        ++idx;
      } else if (isdigit(fmt[idx])) {
        int ref_id = 0;
        for (; idx < fmt.size() && isdigit(fmt[idx]); ++idx)
          ref_id = ref_id * 10 + (fmt[idx] - '0');
        ss << capture[ref_id];
      } else {
        throw std::runtime_error("Unexpected format string.");
      }
    }
  }
  return ss.str();
}

};  // namespace

Machine CreateMachine(const string &e) {
  bool match_begin = StartsWith(e, '^');
  bool match_end = EndsWith(e, '$') && !EndsWith(e, "\\$");

  // Remove positional anchors.
  int begin = match_begin ? 1 : 0;
  int end = match_end ? e.size() - 1 : e.size();
  std::string input = e.substr(begin, end);

  // Create machine with program and positonal match flags.
  RegexpPtr rp = ParseRegexp(input);
  Program program = CompileRegexp(rp);

  Machine m(program);
  m.SetMatchBegin(match_begin);
  m.SetMatchEnd(match_end);
  return m;
}

bool RegexSearch(const Machine &m, const string &s) {
  auto result = m.Run(s, false);
  return result.success;
}

bool RegexSearch(const Machine &m, const string &s, MatchResult &result,
                 bool save_capture) {
  unsigned int offset = result.end;
  if (offset >= s.size()) return false;

  auto temp = m.Run(s.substr(offset), save_capture);
  if (!temp.success) return false;
  temp.begin += offset;
  temp.end += offset;
  result = std::move(temp);
  return result.success;
}

string RegexReplace(const Machine &m, const string &s, const string &fmt,
                    bool global) {
  vector<pair<int, int>> replaced;
  vector<string> new_subs;

  auto ms = m.Run(s, true);
  if (ms.success) {
    replaced.push_back(std::make_pair(ms.begin, ms.end));
    new_subs.push_back(CreateNewSubs(fmt, ms.capture));
  }
  if (global) {
    while (RegexSearch(m, s, ms, true)) {
      replaced.push_back(std::make_pair(ms.begin, ms.end));
      new_subs.push_back(CreateNewSubs(fmt, ms.capture));
    }
  }
  std::stringstream ss;
  int pos = 0;
  for (unsigned idx = 0; idx < replaced.size(); ++idx) {
    auto &p = replaced[idx];
    ss << s.substr(pos, p.first - pos) << new_subs[idx];
    pos = p.second;
  }
  ss << s.substr(pos);
  return ss.str();
}

};  // namespace Azuki
