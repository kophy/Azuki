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

  // Return true if the thread runs one instruction and successfully
  // consumes the input character c.
  // Otherwise, the thread either runs a control instruction and succeeds
  // (it will call AddReadyThread), or it runs a data instruction and fails.
  bool RunOneStep(char c);

 private:
  Machine &machine;
  int pc;
};

// Hold result of regexp match.
struct MatchStatus {
  bool match;
};

// A virtual machine to run program compiled from regexp.
class Machine {
 public:
  Machine(const Program &program);

  void SetMatchBegin(bool b) { match_begin = b; }
  void SetMatchEnd(bool b) { match_end = b; }

  // Run program on input string s.
  MatchStatus Run(const std::string &s);

  // Add a thread to run in this round.
  void AddReadyThread(Thread &&thread) { ready.push(thread); }

  // Update match status.
  void UpdateStatus(bool match) { status.match = match; }

  // Fetch instruction by program counter(index).
  const InstrPtr FetchInstruction(int pc) const { return program[pc]; }

 private:
  const Program program;
  std::queue<Thread> ready;     // keep threads to run in current round
  MatchStatus status;           // keep regex match status

  bool match_begin, match_end;  // control positonal match
};

};  // namespace azuki

#endif
