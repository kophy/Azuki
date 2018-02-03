#ifndef __AZUKI_AZUKI__
#define __AZUKI_AZUKI__

#include "common.h"
#include "machine.h"

namespace Azuki {

// Create machine from raw regular expression.
Machine CreateMachine(const string &e);

// Determines if there is a match between the regular expression represented by
// machine m and some substring in string s.
// If MatchStatus ms is provided, search will begin from ms.end_idx.
// If vector<string> v is provided, captured groups will be saved in v.
bool RegexSearch(const Machine &m, const string &s);
bool RegexSearch(const Machine &m, const string &s, MatchStatus &ms,
                 bool capture = true);

// Replace matched substring in s with new_subs.
string RegexReplace(const Machine &m, const string &s, const string &new_subs,
                    bool global = false);

};  // namespace Azuki

#endif  // __AZUKI_AZUKI__
