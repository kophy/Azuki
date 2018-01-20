#include <boost/spirit/include/classic.hpp>
#include <iostream>
#include "regexp.h"

namespace azuki {

using boost::spirit::classic::grammar;
using boost::spirit::classic::rule;

struct regexp_grammar : public grammar<regexp_grammar> {
  struct print {
    void operator()(const char *begin, const char *end) const {
      std::cout << std::string(begin, end) << std::endl;
    }

    void operator()(const double d) const { std::cout << d << std::endl; }

    void operator()(const char c) const { std::cout << c << std::endl; }
  };

  template <typename Scanner>
  struct definition {
    rule<Scanner> alt, alt1, concat, repeat, single, line;

    definition(const regexp_grammar &self) {
      using namespace boost::spirit::classic;
      line = alt;
      alt = (concat >> '|' >> alt)[print()] | concat;
      concat = +(repeat);
      repeat = single;
      single = alnum_p;
    }

    const rule<Scanner> &start() { return line; }
  };
};

RegexpPtr ParseRegexp(const std::string &s) {
  using namespace boost::spirit::classic;
  regexp_grammar g;
  parse_info<> pi = parse(s.c_str(), g, eol_p);

  if (pi.hit) {
    if (pi.full)
      std::cout << "parsing all data successfully" << std::endl;
    else
      std::cout << "parsing data partially" << std::endl;
    std::cout << pi.length << " characters parsed" << std::endl;
  } else {
    std::cout << "parsing failed; stopped at '" << pi.stop << "'" << std::endl;
  }

  return std::shared_ptr<Regexp>(nullptr);
}  // namespace azuki
};  // namespace azuki
