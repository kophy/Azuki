#ifndef __AZUKI_AZUKI__
#define __AZUKI_AZUKI__

#include <string>
#include <vector>
#include "machine.h"

namespace Azuki {

// Positonal anchors for the begin and end of string.
const char CARET = '^';
const char DOLLAR = '$';

// Create machine from raw regular expression.
Machine CreateMachine(const std::string &e);

// Determines if there is a match between the regular expression represented by
// machine m and some substring in string s.
// If the third parameter vector<string> v is provided, captured groups will
// be saved in v.
bool RegexSearch(const Machine &m, const std::string &s);
bool RegexSearch(const Machine &m, const std::string &s,
                 std::vector<std::string> &v);

};  // namespace Azuki

#endif
