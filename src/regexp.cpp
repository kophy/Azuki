#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include "regexp.h"

namespace azuki {

namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;
namespace qi = boost::spirit::qi;

RegexpPtr CreateRegexp(RegexpType type, char c, RegexpPtr lhs, RegexpPtr rhs) {
  RegexpPtr rp(new Regexp(type, c));
  rp->left = lhs;
  rp->right = rhs;
  return rp;
}

template <typename Iterator>
struct regexp_grammer : qi::grammar<Iterator, RegexpPtr(), ascii::space_type> {
  qi::rule<Iterator, RegexpPtr(), ascii::space_type> regexp;
  qi::rule<Iterator, RegexpPtr(), ascii::space_type> alt;
  qi::rule<Iterator, RegexpPtr(), ascii::space_type> concat;
  qi::rule<Iterator, RegexpPtr(), ascii::space_type> repeat;
  qi::rule<Iterator, RegexpPtr(), ascii::space_type> single;

  regexp_grammer() : regexp_grammer::base_type(regexp) {
    using namespace qi;
    regexp = alt[_val = _1];
    alt = (concat >> "|" >>
           alt)[_val = phx::bind(CreateRegexp, ALT, ' ', _1, _2)] |
          concat[_val = _1];
    concat =
        (repeat >> concat)[_val = phx::bind(CreateRegexp, CAT, ' ', _1, _2)] |
        repeat[_val = _1];
    repeat =
        (single >> ",")[_val = phx::bind(CreateRegexp, DOT, ' ', _1, nullptr)] |
        (single >>
         "+")[_val = phx::bind(CreateRegexp, PLUS, ' ', _1, nullptr)] |
        (single >>
         "?")[_val = phx::bind(CreateRegexp, QUEST, ' ', _1, nullptr)] |
        (single >>
         "*")[_val = phx::bind(CreateRegexp, STAR, ' ', _1, nullptr)] |
        single[_val = _1];
    single =
        alnum[_val = phx::bind(CreateRegexp, LIT, _1, nullptr, nullptr)] |
        char_('.')[_val = phx::bind(CreateRegexp, DOT, ' ', nullptr, nullptr)];
  }
};

RegexpPtr ParseRegexp(const std::string &s) {
  typedef std::string::const_iterator Iterator;
  regexp_grammer<Iterator> g;
  RegexpPtr r;

  Iterator begin(s.begin()), end(s.end());
  bool ok = qi::phrase_parse(begin, end, g, ascii::space, r);
  if (ok) return r;
  throw std::exception();
  ;
}

void PrintRegexpHelper(int tab, RegexpPtr rp) {
  std::cout << std::string(tab * 4, ' ');
  switch (rp->type) {
    case LIT:
      std::cout << "LIT " << rp->c << std::endl;
      break;
    case ALT:
      std::cout << "ALT" << std::endl;
      PrintRegexpHelper(tab + 1, rp->left);
      PrintRegexpHelper(tab + 1, rp->right);
      break;
    case CAT:
      std::cout << "CAT" << std::endl;
      PrintRegexpHelper(tab + 1, rp->left);
      PrintRegexpHelper(tab + 1, rp->right);
      break;
    case DOT:
      std::cout << "DOT" << std::endl;
      break;
    case PLUS:
      std::cout << "PLUS" << std::endl;
      PrintRegexpHelper(tab + 1, rp->left);
      break;
    case QUEST:
      std::cout << "QUEST" << std::endl;
      PrintRegexpHelper(tab + 1, rp->left);
      break;
    case STAR:
      std::cout << "STAR" << std::endl;
      PrintRegexpHelper(tab + 1, rp->left);
      break;
    default:
      break;
  }
}

void PrintRegexp(RegexpPtr rp) { PrintRegexpHelper(0, rp); }

};  // namespace azuki
