#include <iostream>
#include "azuki.h"

int main() {
  Azuki::Machine m = Azuki::CreateMachine(
      "^(\\+\\d{1,2}\\s)?\\(?\\d{3}\\)?(\\s|-|.)\\d{3}(\\s|-|.)\\d{4}$");

  std::vector<std::string> input = {"123-456-7890", "(123) 456-7890",
                                    "123 456 7890", "123.456.7890"};
  for (auto &s : input) {
    std::cout << s << "\t";
    if (Azuki::RegexSearch(m, s))
      std::cout << "YES" << std::endl;
    else
      std::cout << "NO" << std::endl;
  }
  return 0;
}
