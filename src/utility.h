#ifndef __AZUKI_UTILITY__
#define __AZUKI_UTILITY__

#include <string>

namespace azuki {

bool StartsWith(const std::string &s, char prefix);
bool StartsWith(const std::string &s, const std::string &prefix);

bool EndsWith(const std::string &s, char suffix);
bool EndsWith(const std::string &s, const std::string &suffix);

};  // namespace azuki

#endif
