#ifndef __AZUKI_UTILITY__
#define __AZUKI_UTILITY__

#include "common.h"

namespace Azuki {

// Check whether string s starts with prefix.
bool StartsWith(const string &s, char prefix);
bool StartsWith(const string &s, const string &prefix);

// Check whether string s ends with suffix.
bool EndsWith(const string &s, char suffix);
bool EndsWith(const string &s, const string &suffix);

};  // namespace Azuki

#endif  // __AZUKI_UTILITY__
