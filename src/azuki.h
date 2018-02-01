#ifndef __AZUKI_AZUKI__
#define __AZUKI_AZUKI__

#include "common.h"
#include "machine.h"

namespace Azuki {

// Positonal anchors for the begin and end of string.
const char CARET = '^';
const char DOLLAR = '$';

// Create machine from raw regular expression.
Machine CreateMachine(const string &e);

// Determines if there is a match between the regular expression represented by
// machine m and some substring in string s.
// If MatchStatus ms is provided, search will begin from ms.end_idx.
// If vector<string> v is provided, captured groups will be saved in v.
bool RegexSearch(const Machine &m, const string &s);
bool RegexSearch(const Machine &m, const string &s, vector<string> &v);
bool RegexSearch(const Machine &m, const string &s, MatchStatus &ms);
bool RegexSearch(const Machine &m, const string &s, MatchStatus &ms,
                 vector<string> &v);

// Replace matched substring in s with new_subs.
string RegexReplace(const Machine &m, const string &s, const string &new_subs,
                    bool global = false);

};  // namespace Azuki

#endif
