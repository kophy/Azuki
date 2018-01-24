#include <iostream>
#include <string>
#include <vector>
#include "azuki.h"

int main() {
  Azuki::Machine m = Azuki::CreateMachine("(23+)");
  std::vector<std::string> v;
  if (Azuki::RegexSearch(m, "6662333QAQ", v)) {
    for (auto &s : v)
      std::cout << s << std::endl;
  }
  return 0;
}
