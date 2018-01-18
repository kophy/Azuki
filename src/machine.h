#ifndef __AZUKI_MACHINE__
#define __AZUKI_MACHINE__

#include <memory>
#include <queue>
#include <string>
#include "instruction.h"

namespace azuki {

class Machine;

class Thread {
 public:
  Thread(Machine &machine, int pc) : machine(machine), pc(pc) {}

  // Return true if in this one step, this thread successfully consumes the
  // given character c.
  // Otherwise,
  bool RunOneStep(char c);

 private:
  Machine &machine;
  int pc;
};

// Hold result of regexp match.
struct MatchStatus {
  bool match;
};

// A virtual machine to run Thompson's algorithm.
class Machine {
 public:
  Machine(const Program &program) : program(program) {}

  // Run program on input string s.
  MatchStatus Run(const std::string &s);

  // Add a thread to run in this round.
  void AddReadyThread(Thread &&thread) { ready.push(thread); }

  void UpdateStatus(bool match) { status.match = match; }

  const InstrPtr FetchInstruction(int pc) const { return program[pc]; }

 private:
  const Program program;
  std::queue<Thread> ready;  // set of threads to run in current round
  MatchStatus status;        // keep regex match status
};

};  // namespace azuki

#endif
