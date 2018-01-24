#ifndef __AZUKI_MACHINE__
#define __AZUKI_MACHINE__

#include <memory>
#include <queue>
#include <string>
#include <vector>
#include "instruction.h"

namespace Azuki {

// Hold result of regexp match.
struct MatchStatus {
  bool match;
  std::vector<std::string::const_iterator> saved;  // save capturing groups
};

class Machine;

// The Thread class implements threads to be run in the virtual machine.
// Each thread keeps its own program counter and match status.
class Thread {
 public:
  Thread(const Machine &machine, int pc) : machine(machine), pc(pc) {}
  Thread(const Machine &machine, int pc, const MatchStatus &status)
      : machine(machine), pc(pc), status(status) {}

  // Return true if the thread runs one instruction and successfully
  // consumes the input character referenced by sp.
  // Otherwise, the thread either runs a control instruction and succeeds
  // (it will call "AddReadyThread"), or runs a data instruction and fails.
  bool RunOneStep(std::string::const_iterator sp, bool capture = true);

 private:
  const Machine &machine;  // host machine
  int pc;                  // program counter
  MatchStatus status;      // this thread's match status
};

// The Machine class implements a virtual machine to run Thompson's algorithm.
// It maintains a collection of threads ready to run, and these threads are run
// in lock step -- all threads process the same character in each iteration,
// even new threads added in current iteration through "AddReadyThread".
class Machine {
 public:
  Machine(const Program &program);

  // Set flags to control positional match.
  Machine &SetMatchBegin(bool b);
  Machine &SetMatchEnd(bool b);

  // Run program on input string s (with Rob Pike's implementation).
  MatchStatus Run(const std::string &s, bool capture = true) const;

  // Add a thread to run in this round.
  void AddReadyThread(Thread &&thread) const { ready.push(thread); }

  // Update match status.
  void UpdateStatus(const MatchStatus &status) const;

  // Fetch instruction by program counter(index).
  const InstrPtr FetchInstruction(int pc) const { return program[pc]; }

 private:
  const Program program;
  mutable std::queue<Thread> ready;  // keep threads to run in current iteration
  mutable MatchStatus status;        // keep regex match status
  bool match_begin, match_end;       // flags to control positonal match
};

};  // namespace Azuki

#endif
