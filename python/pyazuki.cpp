#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "azuki.h"
#include "common.h"
#include "machine.h"

BOOST_PYTHON_MODULE(pyazuki) {
  using namespace Azuki;
  using namespace boost::python;

  class_<Regexp>("Regexp");
  class_<RegexpPtr>("RegexpPtr");
  def("ParseRegexp", ParseRegexp);
  def("PrintRegexp", PrintRegexp);

  class_<Instruction>("instruction");
  class_<Program>("Program");
  def("CompileRegexp", CompileRegexp);
  def("PrintProgram", PrintProgram);

  class_<MatchResult>("MatchResult")
      .def_readonly("begin", &MatchResult::begin)
      .def_readonly("end", &MatchResult::end)
      .def_readonly("capture", &MatchResult::capture);

  class_<Machine>("Machine", init<const Program &>())
      .def("SetMatchBegin", &Machine::SetMatchBegin)
      .def("SetMatchEnd", &Machine::SetMatchEnd)
      .def("Run", &Machine::Run);

  class_<vector<string>>("vector<string>")
      .def(vector_indexing_suite<vector<string>>())
      .def("size", &vector<string>::size);

  def("CreateMachine", CreateMachine);
  def("RegexSearch",
      static_cast<bool (*)(const Machine &, const string &)>(RegexSearch));
  def("RegexSearch", static_cast<bool (*)(const Machine &, const string &,
                                          MatchResult &, bool)>(RegexSearch));
  def("RegexReplace", RegexReplace);
}
