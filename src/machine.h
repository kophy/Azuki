#ifndef __AZUKI_MACHINE__
#define __AZUKI_MACHINE__

#include <queue>
#include "common.h"
#include "instruction.h"

namespace Azuki {

// The MatchStatus struct is used to hold results of regexp match.
struct MatchStatus {
  bool success;
  unsigned int begin_idx;
  unsigned int end_idx;
  vector<StringPtr> saved;  // saved capturing groups
  vector<int> repeated;     // counters of repeat times of subexpressions

  MatchStatus() : success(false) {}
};

class Machine;  // forward declaration

// The Thread class implements threads to be run in the virtual machine.
// Each thread keeps its own program counter and match status.
class Thread : public std::enable_shared_from_this<Thread> {
 public:
  Thread(const Machine &machine, int pc, unsigned int begin_idx);

  // Create a new thread with exact same state as this thread but a different
  // program counter.
  shared_ptr<Thread> Split(int other_pc);

  // Return true if the thread runs one instruction and successfully
  // consumes the input character referenced by sp.
  // Otherwise, the thread either runs a control instruction and succeeds
  // (it will call "AddReadyThread"), or runs a data instruction and fails.
  bool RunOneStep(StringPtr sp, bool capture = true);

 private:
  const Machine &machine;  // host machine
  int pc;                  // program counter
  MatchStatus status;      // this thread's match status
};

typedef shared_ptr<Thread> ThreadPtr;

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
  MatchStatus Run(const string &s, bool capture = true) const;

  // Add a thread to run in this round.
  void AddReadyThread(ThreadPtr tp) const { ready.push(tp); }

  // Update match status.
  void UpdateStatus(const MatchStatus &status_) const;

  // Fetch instruction by program counter(index).
  const InstrPtr FetchInstruction(int pc) const { return program[pc]; }

 private:
  const Program program;
  mutable std::queue<ThreadPtr> ready;  // threads to run in current iteration
  mutable MatchStatus status;           // match status
  bool match_begin, match_end;          // flags to control positonal match
};

};  // namespace Azuki

#endif  // __AZUKI_MACHINE__
