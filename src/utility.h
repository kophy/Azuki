#ifndef __AZUKI_UTILITY__
#define __AZUKI_UTILITY__

#include <string>

namespace Azuki {

// Check whether string s starts with prefix.
bool StartsWith(const std::string &s, char prefix);
bool StartsWith(const std::string &s, const std::string &prefix);

// Check whether string s ends with suffix.
bool EndsWith(const std::string &s, char suffix);
bool EndsWith(const std::string &s, const std::string &suffix);

};  // namespace Azuki

#endif
