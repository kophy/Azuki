#include <iostream>
#include <string>
#include <vector>
#include "azuki.h"

int main() {
  Azuki::Machine m = Azuki::CreateMachine("www\\.(\\w+)\\.com");
  Azuki::MatchStatus ms;
  std::vector<std::string> v;
  if (Azuki::RegexSearch(m, "https://www.google.com/search?q=Hello", ms)) {
    std::cout << "success" << std::endl;
    for (auto &s : ms.captured)
      std::cout << s << std::endl;
  } else {
    std::cout << "failure" << std::endl;
  }
  return 0;
}
