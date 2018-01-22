#ifndef __AZUKI_AZUKI__
#define __AZUKI_AZUKI__

#include <string>
#include "machine.h"

namespace azuki {

const char CARET = '^';
const char DOLLAR = '$';

// Create machine from raw regular expression e.
Machine CreateMachine(const std::string &e);

bool RegexSearch(const Machine &m, const std::string &s);

};  // namespace azuki

#endif
