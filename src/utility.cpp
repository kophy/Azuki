#include <algorithm>
#include "utility.h"

namespace Azuki {

bool StartsWith(const string &s, char prefix) {
  return s.size() && s.front() == prefix;
}

bool StartsWith(const string &s, const string &prefix) {
  if (s.size() < prefix.size()) return false;
  return std::equal(prefix.begin(), prefix.end(), s.begin());
}

bool EndsWith(const string &s, char suffix) {
  return s.size() && s.back() == suffix;
}

bool EndsWith(const string &s, const string &suffix) {
  if (s.size() < suffix.size()) return false;
  return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

};  // namespace Azuki
