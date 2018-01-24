#include <algorithm>
#include <sstream>
#include "utility"

namespace azuki {

bool StartsWith(const std::string &s, char prefix) {
  return s.size() && s.front() == prefix;
}

bool StartsWith(const std::string &s, const std::string &prefix) {
  if (s.size() < prefix.size()) return false;
  return std::equal(prefix.begin(), prefix.end(), s.begin());
}

bool EndsWith(const std::string &s, char suffix) {
  return s.size() && s.back() == suffix;
}

bool EndsWith(const std::string &s, const std::string &suffix) {
  if (s.size() < suffix.size()) return false;
  return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

};  // namespace azuki
