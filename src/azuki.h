#ifndef __AZUKI_AZUKI__
#define __AZUKI_AZUKI__

#include "common.h"
#include "machine.h"

namespace Azuki {

// Create machine from raw regular expression. The regular expression can
// contain positional symbol '^' and '$'.
// Example:
//    Machine m = CreateMachine("^a+b$");
Machine CreateMachine(const string &e);

// Determines if there is a match between the regular expression represented by
// machine m and some substring in string s.
// Example:
//    Machine m = CreateMachine("^a+b$");
//    RegexSearch(m, "aaab");  // true
//    RegexSearch(m, "ac");    // false
bool RegexSearch(const Machine &m, const string &s);

// Determines if there is a match between the regular expression represented by
// machine m and some substring in string s.
// Search works on the substring of s, begining from ms.end.
// If save_capture is true, capture groups will be saved in result.capture.
// Example:
//    Machine m = CreateMachine("(a+b)");
//    MatchResult result;
//    result.end = 1;
//    RegexSearch(m, "abcaabd", result, true); // true
// Then we have:
//    result.begin = 3, result.end = 6, result.capture = {"aab"}
bool RegexSearch(const Machine &m, const string &s, MatchResult &result,
                 bool save_capture = true);

// Replace matched substring in s with new substring specified by format string
// fmt. Backreference is supported with "$0", "$1", etc. Use "$$" for a single
// '$' character.
// If replace_global is true, it will replace all match substrings. Otherwise,
// it will replace only the first match.
// Example:
//    Machine m = CreateMachine("(a+)b");
//    RegexReplace(m, "aab", "$0c");  // "aac"
string RegexReplace(const Machine &m, const string &s, const string &fmt,
                    bool replace_global = false);

};  // namespace Azuki

#endif  // __AZUKI_AZUKI__
