#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include "regexp.h"

namespace azuki {

namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;
namespace qi = boost::spirit::qi;

Regexp CreateRegexp(RegexpType type, char c, const Regexp &left,
                    const Regexp &right) {
  Regexp r;
  r.type = type;
  r.c = c;
  r.left = std::make_shared<Regexp>(left);
  r.right = std::make_shared<Regexp>(right);
  return r;
}

template <typename Iterator>
struct regexp_grammer : qi::grammar<Iterator, Regexp(), ascii::space_type> {
  qi::rule<Iterator, Regexp(), ascii::space_type> regexp;
  qi::rule<Iterator, Regexp(), ascii::space_type> alt;
  qi::rule<Iterator, Regexp(), ascii::space_type> concat;
  qi::rule<Iterator, Regexp(), ascii::space_type> repeat;
  qi::rule<Iterator, Regexp(), ascii::space_type> single;

  regexp_grammer() : regexp_grammer::base_type(regexp) {
    using namespace qi;
    regexp = alt[_val = _1];
    alt = (single >> "|" >>
           alt)[_val = phx::bind(CreateRegexp, ALT, ' ', _1, _2)] |
          single[_val = _1];
    single = +(alnum)[_val = Regexp(LIT)];
  }
};

RegexpPtr ParseRegexp(const std::string &s) {
  typedef std::string::const_iterator Iterator;
  regexp_grammer<Iterator> g;
  Regexp r;

  Iterator begin(s.begin()), end(s.end());
  bool ok = qi::phrase_parse(begin, end, g, ascii::space, r);

  std::cout << ok << std::endl;
  // std::cout << std::string(begin, end) << std::endl;
  PrintRegex(std::make_shared<Regexp>(r));

  return std::shared_ptr<Regexp>(nullptr);
}

void PrintRegexHelper(int tab, RegexpPtr rp) {
  std::cout << std::string(tab, ' ');
  switch (rp->type) {
    case LIT:
      std::cout << "LIT" << std::endl;
      break;
    case ALT:
      std::cout << "ALT" << std::endl;
      PrintRegexHelper(1, rp->left);
      PrintRegexHelper(1, rp->right);
      break;
    default:
      break;
  }
}

void PrintRegex(RegexpPtr rp) { PrintRegexHelper(0, rp); }

};  // namespace azuki
