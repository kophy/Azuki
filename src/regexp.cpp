#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include "regexp.h"

namespace azuki {

namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;
namespace qi = boost::spirit::qi;

RegexpPtr CreateALTRegexp(RegexpPtr left, RegexpPtr right) {
  RegexpPtr rp(new Regexp(ALT));
  rp->left = left;
  rp->right = right;
  return rp;
}

RegexpPtr CreateCATRegexp(RegexpPtr left, RegexpPtr right) {
  RegexpPtr rp(new Regexp(CAT));
  rp->left = left;
  rp->right = right;
  return rp;
}

RegexpPtr CreateDOTRegexp() {
  RegexpPtr rp(new Regexp(DOT));
  return rp;
}

RegexpPtr CreateLITRegexp(char c) {
  RegexpPtr rp(new Regexp(LIT, c));
  return rp;
}

RegexpPtr CreatePARENRegexp(RegexpPtr left) {
  RegexpPtr rp(new Regexp(PAREN));
  rp->left = left;
  return rp;
}

RegexpPtr CreatePLUSRegexp(RegexpPtr left) {
  RegexpPtr rp(new Regexp(PLUS));
  rp->left = left;
  return rp;
}

RegexpPtr CreateQUESTRegexp(RegexpPtr left) {
  RegexpPtr rp(new Regexp(QUEST));
  rp->left = left;
  return rp;
}

RegexpPtr CreateSTARRegexp(RegexpPtr left) {
  RegexpPtr rp(new Regexp(STAR));
  rp->left = left;
  return rp;
}

RegexpPtr CreateRegexp(RegexpType type, char c, RegexpPtr left,
                       RegexpPtr right) {
  RegexpPtr rp(new Regexp(type, c));
  rp->left = left;
  rp->right = right;
  return rp;
}

template <typename Iterator>
struct regexp_grammer : qi::grammar<Iterator, RegexpPtr()> {
  qi::rule<Iterator, RegexpPtr()> regexp;
  qi::rule<Iterator, RegexpPtr()> alt;
  qi::rule<Iterator, RegexpPtr()> concat;
  qi::rule<Iterator, RegexpPtr()> repeat;
  qi::rule<Iterator, RegexpPtr()> single;

  regexp_grammer() : regexp_grammer::base_type(regexp) {
    using namespace qi;

    regexp = alt[_val = _1];
    alt = (concat >> "|" >> alt)[_val = phx::bind(CreateALTRegexp, _1, _2)] |
          concat[_val = _1];
    concat = (repeat >> concat)[_val = phx::bind(CreateCATRegexp, _1, _2)] |
             repeat[_val = _1];
    repeat = (single >> "+")[_val = phx::bind(CreatePLUSRegexp, _1)] |
             (single >> "?")[_val = phx::bind(CreateQUESTRegexp, _1)] |
             (single >> "*")[_val = phx::bind(CreateSTARRegexp, _1)] |
             single[_val = _1];
    single = ("(" >> regexp >> ")")[_val = phx::bind(CreatePARENRegexp, _1)] |
             (alnum)[_val = phx::bind(CreateLITRegexp, _1)] |
             (space)[_val = phx::bind(CreateLITRegexp, _1)] |
             char_('.')[_val = CreateDOTRegexp()];
  }
};

RegexpPtr ParseRegexp(const std::string &s) {
  typedef std::string::const_iterator Iterator;
  regexp_grammer<Iterator> g;
  RegexpPtr rp;

  bool ok = qi::phrase_parse(s.begin(), s.end(), g, ascii::space, rp);
  if (ok && IsValidRegexp(rp)) return rp;
  throw std::exception();
}

void PrintRegexpImpl(int tab, RegexpPtr rp) {
  std::cout << std::string(tab * 2, ' ');
  switch (rp->type) {
    case LIT:
      std::cout << "LIT " << rp->c << std::endl;
      break;
    case ALT:
      std::cout << "ALT" << std::endl;
      PrintRegexpImpl(tab + 1, rp->left);
      PrintRegexpImpl(tab + 1, rp->right);
      break;
    case CAT:
      std::cout << "CAT" << std::endl;
      PrintRegexpImpl(tab + 1, rp->left);
      PrintRegexpImpl(tab + 1, rp->right);
      break;
    case DOT:
      std::cout << "DOT" << std::endl;
      break;
    case PAREN:
      std::cout << "PAREN" << std::endl;
      PrintRegexpImpl(tab + 1, rp->left);
      break;
    case PLUS:
      std::cout << "PLUS" << std::endl;
      PrintRegexpImpl(tab + 1, rp->left);
      break;
    case QUEST:
      std::cout << "QUEST" << std::endl;
      PrintRegexpImpl(tab + 1, rp->left);
      break;
    case STAR:
      std::cout << "STAR" << std::endl;
      PrintRegexpImpl(tab + 1, rp->left);
      break;
    default:
      break;
  }
}

void PrintRegexp(RegexpPtr rp) { PrintRegexpImpl(0, rp); }

// TODO: add serious check
bool IsValidRegexp(RegexpPtr rp) { return true; }

};  // namespace azuki
