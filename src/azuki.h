#ifndef __AZUKI_AZUKI__
#define __AZUKI_AZUKI__

#include <string>
#include <vector>
#include "machine.h"

namespace azuki {

const char CARET = '^';
const char DOLLAR = '$';

// Create machine from raw regular expression e.
Machine CreateMachine(const std::string &e);

bool RegexSearch(const Machine &m, const std::string &s);

bool RegexMatch(const Machine &m, const std::string &s,
                std::vector<std::string> &v);

};  // namespace azuki

#endif
