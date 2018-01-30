#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <climits>
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

RegexpPtr CreateSquareRegexp(char low_ch, char high_ch) {
  RegexpPtr rp(new Regexp());
  rp->type = SQUARE;
  rp->low_ch = low_ch;
  rp->high_ch = high_ch;
  return rp;
}

RegexpPtr CreateCurlyRegexp(RegexpPtr left, int low_times, int high_times) {
  RegexpPtr rp(new Regexp());
  rp->type = CURLY;
  rp->left = left;
  rp->low_times = low_times;
  rp->high_times = high_times;
  return rp;
}

RegexpPtr CreateRegexpWithEscaped(char c) {
  // \w for wrod, \d for digit, \s for space
  static std::string class_char = "dsw";
  // only character used as operators can be escaped
  static std::string special_char = ".+?*|\\()[]{}";

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
  qi::rule<Iterator, int()> number;

  regexp_grammer() : regexp_grammer::base_type(regexp) {
    using namespace qi;

    regexp = alt[_val = _1];
    alt = (concat >> "|" >> alt)[_val = phx::bind(CreateAltRegexp, _1, _2)] |
          concat[_val = _1];
    concat = (repeat >> concat)[_val = phx::bind(CreateCatRegexp, _1, _2)] |
             repeat[_val = _1];
    repeat =
        (single >> "{" >> int_ >>
         "}")[_val = phx::bind(CreateCurlyRegexp, _1, _2, _2)] |
        (single >> "{" >> int_ >> "," >> int_ >>
         "}")[_val = phx::bind(CreateCurlyRegexp, _1, _2, _3)] |
        (single >> "{" >> int_ >>
         ",}")[_val = phx::bind(CreateCurlyRegexp, _1, _2, phx::val(INT_MAX))] |
        (single >> "+")[_val = phx::bind(CreatePlusRegexp, _1)] |
        (single >> "?")[_val = phx::bind(CreateQuestRegexp, _1)] |
        (single >> "*")[_val = phx::bind(CreateStarRegexp, _1)] |
        single[_val = _1];
    single = ("(" >> regexp >> ")")[_val = phx::bind(CreateParenRegexp, _1)] |
             ("[" >> char_ >> "-" >> char_ >>
              "]")[_val = phx::bind(CreateSquareRegexp, _1, _2)] |
             ("\\" >> char_)[_val = phx::bind(CreateRegexpWithEscaped, _1)] |
             (alnum)[_val = phx::bind(CreateLitRegexp, _1)] |
             (space)[_val = phx::bind(CreateLitRegexp, _1)] |
             (char_("~!@#%&=:;,_<>-"))[_val = phx::bind(CreateLitRegexp, _1)] |
             char_('.')[_val = CreateDotRegexp()];
    number = int_;
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
    case CURLY:
      std::cout << "CURLY " << rp->low_times << " " << rp->high_times
                << std::endl;
      PrintRegexpImpl(tab + 1, rp->left);
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
    case SQUARE:
      std::cout << "SQUARE " << rp->low_ch << " " << rp->high_ch << std::endl;
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
      case SQUARE:
        return rp->low_ch <= rp->high_ch;
      case CURLY:
        return rp->low_times <= rp->high_times;
      default:
        std::cerr << "Unexpected regexp type." << std::endl;
        return false;
    }
  }
  return false;
}

bool operator==(RegexpPtr rp1, RegexpPtr rp2) {
  if (rp1->type != rp2->type) return false;
  switch (rp1->type) {
    case ALT:
    case CAT:
      return rp1->left == rp2->left && rp1->right == rp2->right;
    case CLASS:
      return rp1->c == rp2->c;
    case CURLY:
      return rp1->low_times == rp2->low_times &&
             rp1->high_times == rp2->high_times && rp1->left == rp2->left;
    case DOT:
      return true;
    case PAREN:
    case PLUS:
    case QUEST:
    case STAR:
      return rp1->left == rp2->left;
    case LIT:
      return rp1->c == rp2->c;
    case SQUARE:
      return rp1->low_ch == rp2->low_ch && rp1->high_ch == rp2->high_ch;
    default:
      std::cerr << "Unexpected regexp type." << std::endl;
      return false;
  }
  return false;
}

};  // namespace Azuki
