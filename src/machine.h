#ifndef __AZUKI_MACHINE__
#define __AZUKI_MACHINE__

#include <memory>
#include <queue>
#include <string>
#include <vector>
#include "instruction.h"

namespace azuki {

// Hold result of regexp match.
struct MatchStatus {
  bool match;
  std::vector<std::string::const_iterator> saved;
};

class Machine;

// TODO: add comment
class Thread {
 public:
  Thread(const Machine &machine, int pc) : machine(machine), pc(pc) {}
  Thread(const Machine &machine, int pc,
         const std::vector<std::string::const_iterator> &saved)
      : machine(machine), pc(pc), saved(saved) {}

  // Return true if the thread runs one instruction and successfully
  // consumes the input character referenced by string pointer sp.
  // Otherwise, the thread either runs a control instruction and succeeds
  // (it will call AddReadyThread), or it runs a data instruction and fails.
  bool RunOneStep(std::string::const_iterator sp);

  std::vector<std::string::const_iterator> GetSaved() {return saved;}

 private:
  const Machine &machine;
  int pc;
  std::vector<std::string::const_iterator> saved;
};

// A virtual machine to run program compiled from regexp.
class Machine {
 public:
  Machine(const Program &program);

  // Control positional match.
  void SetMatchBegin(bool b) { match_begin = b; }
  void SetMatchEnd(bool b) { match_end = b; }

  // Run program on input string s.
  MatchStatus Run(const std::string &s) const;

 private:
  friend class Thread;

  // Add a thread to run in this round.
  void AddReadyThread(Thread &&thread) const { ready.push(thread); }

  // Update match status.
  void UpdateStatus(bool match) const { status.match = match; }

  // Fetch instruction by program counter(index).
  const InstrPtr FetchInstruction(int pc) const { return program[pc]; }

 private:
  const Program program;
  mutable std::queue<Thread> ready;  // keep threads to run in current round
  mutable MatchStatus status;        // keep regex match status

  bool match_begin, match_end;  // control positonal match
};

};  // namespace azuki

#endif
