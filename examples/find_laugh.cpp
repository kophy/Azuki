#include <iostream>
#include <string>
#include <vector>
#include "azuki.h"

int main() {
  Azuki::Machine m = Azuki::CreateMachine("(233+)");
  std::vector<std::string> v;
  if (Azuki::RegexSearch(m, "6662333QAQ", v)) {
    std::cout << "success" << std::endl;
    for (auto &s : v)
      std::cout << s << std::endl;
  } else {
    std::cout << "failure" << std::endl;
  }
  return 0;
}
