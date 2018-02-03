#include <iostream>
#include <string>
#include <vector>
#include "azuki.h"

int main() {
  Azuki::Machine m = Azuki::CreateMachine("(233+)");
  Azuki::MatchResult ms;
  std::vector<std::string> v;
  if (Azuki::RegexSearch(m, "6662333QAQ", ms)) {
    std::cout << "success" << std::endl;
    for (auto &s : ms.capture)
      std::cout << s << std::endl;
  } else {
    std::cout << "failure" << std::endl;
  }
  return 0;
}
