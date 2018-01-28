#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include "regexp.h"

namespace Azuki {

namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;
namespace qi = boost::spirit::qi;

// A wrapper to handle arbitrary escaped character.
// Either CreateClassRegexp or CreateLitRegexp is called to create the Regexp.
RegexpPtr CreateRegexpWithEscaped(char c);

RegexpPtr CreateAltRegexp(RegexpPtr left, RegexpPtr right) {
  RegexpPtr rp(new Regexp());
  rp->type = ALT;
  rp->left = left;
  rp->right = right;
  return rp;
}

RegexpPtr CreateCatRegexp(RegexpPtr left, RegexpPtr right) {
  RegexpPtr rp(new Regexp());
  rp->type = CAT;
  rp->left = left;
  rp->right = right;
  return rp;
}

RegexpPtr CreateClassRegexp(char c) {
  RegexpPtr rp(new Regexp());
  rp->type = CLASS;
  rp->c = c;
  return rp;
}

RegexpPtr CreateDotRegexp() {
  RegexpPtr rp(new Regexp());
  rp->type = DOT;
  return rp;
}

RegexpPtr CreateLitRegexp(char c) {
  RegexpPtr rp(new Regexp());
  rp->type = LIT;
  rp->c = c;
  return rp;
}

RegexpPtr CreateParenRegexp(RegexpPtr left) {
  RegexpPtr rp(new Regexp());
  rp->type = PAREN;
  rp->left = left;
  return rp;
}

RegexpPtr CreatePlusRegexp(RegexpPtr left) {
  RegexpPtr rp(new Regexp());
  rp->type = PLUS;
  rp->left = left;
  return rp;
}

RegexpPtr CreateQuestRegexp(RegexpPtr left) {
  RegexpPtr rp(new Regexp());
  rp->type = QUEST;
  rp->left = left;
  return rp;
}

RegexpPtr CreateStarRegexp(RegexpPtr left) {
  RegexpPtr rp(new Regexp());
  rp->type = STAR;
  rp->left = left;
  return rp;
}

RegexpPtr CreateRegexpWithEscaped(char c) {
  // \w for wrod, \d for digit, \s for space
  static std::string class_char = "dsw";
  // only character used as operators can be escaped
  static std::string special_char = ".+?*|\\()[]";

  if (class_char.find(c) != std::string::npos)
    return CreateClassRegexp(c);
  else if (special_char.find(c) != std::string::npos)
    return CreateLitRegexp(c);
  else
    throw std::runtime_error("Invalid escaped character.");
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
    alt = (concat >> "|" >> alt)[_val = phx::bind(CreateAltRegexp, _1, _2)] |
          concat[_val = _1];
    concat = (repeat >> concat)[_val = phx::bind(CreateCatRegexp, _1, _2)] |
             repeat[_val = _1];
    repeat = (single >> "+")[_val = phx::bind(CreatePlusRegexp, _1)] |
             (single >> "?")[_val = phx::bind(CreateQuestRegexp, _1)] |
             (single >> "*")[_val = phx::bind(CreateStarRegexp, _1)] |
             single[_val = _1];
    single = ("(" >> regexp >> ")")[_val = phx::bind(CreateParenRegexp, _1)] |
             ("\\" >> char_)[_val = phx::bind(CreateRegexpWithEscaped, _1)] |
             (alnum)[_val = phx::bind(CreateLitRegexp, _1)] |
             (space)[_val = phx::bind(CreateLitRegexp, _1)] |
             (char_("~!@#%&=:;,_<>-"))[_val = phx::bind(CreateLitRegexp, _1)] |
             char_('.')[_val = CreateDotRegexp()];
  }
};

RegexpPtr ParseRegexp(const std::string &s) {
  typedef std::string::const_iterator Iterator;

  regexp_grammer<Iterator> g;
  RegexpPtr rp;

  bool ok = qi::phrase_parse(s.begin(), s.end(), g, ascii::space, rp);
  if (ok && IsValidRegexp(rp)) return rp;
  throw std::runtime_error("Invalid regular expression.");
}

void PrintRegexpImpl(int tab, RegexpPtr rp) {
  if (tab > 0) std::cout << string((tab - 1) * 4, ' ') << "|--";
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
    case CLASS:
      std::cout << "CLASS " << rp->c << std::endl;
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
      throw std::runtime_error("Unexpected regexp type.");
  }
}

void PrintRegexp(RegexpPtr rp) { PrintRegexpImpl(0, rp); }

bool IsValidRegexp(RegexpPtr rp) noexcept {
  if (rp.get()) {
    switch (rp->type) {
      case ALT:
      case CAT:
        return IsValidRegexp(rp->left) && IsValidRegexp(rp->right);
      case CLASS:
      case LIT:
      case DOT:
        return true;
      case PAREN:
      case PLUS:
      case QUEST:
      case STAR:
        return IsValidRegexp(rp->left);
      default:
        std::cerr << "Unexpected regexp type." << std::endl;
        return false;
    }
  }
  return false;
}

};  // namespace Azuki
