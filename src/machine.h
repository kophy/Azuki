#ifndef __AZUKI_MACHINE__
#define __AZUKI_MACHINE__

#include <queue>
#include "common.h"
#include "instruction.h"

namespace Azuki {

// The MatchStatus struct holds results of regexp match.
struct MatchStatus {
  bool success;
  unsigned int begin, end;
  vector<string> captured;

  MatchStatus();
};

class Machine;  // forward declaration

// The Thread class implements "fake" threads to run in the virtual machine.
// Each thread keeps its own program counter and match status.
class Thread : public std::enable_shared_from_this<Thread> {
 public:
  struct Status {
    unsigned int begin, end;
    vector<StringPtr> saved;
    vector<int> repeated;
  };

 public:
  Thread(const Machine &machine, int pc, unsigned int begin);

  // Create a new thread with exact same state as this thread but a different
  // program counter.
  // Example:
  //    ThreadPtr tp1(new Thread(machine, 0, 0));
  //    ThreadPtr tp2 = ThreadPtr(tp1->Split(2));
  // Then we have tp1->machine == tp2->machine, tp1->status == tp2->status,
  // but tp1->pc != tp2->pc.
  Thread *Split(int other_pc);

  // Return true if the thread runs one instruction and successfully
  // consumes the input character referenced by sp. The thread should be run
  // in next round.
  // Otherwise, the thread either runs a control instruction and succeeds, or
  // runs a data instruction and fails. In both cases, the thread should not
  // be run in next round.
  // If capture is true, then capture groups will be saved.
  bool RunOneStep(StringPtr sp, bool capture = true);

 private:
  const Machine &machine;  // host machine
  unsigned int pc;         // program counter
  Thread::Status tstatus;  // this thread's match status
};

typedef shared_ptr<Thread> ThreadPtr;

// The Machine class implements a virtual machine to run Thompson's algorithm.
// Example:
//    Machine machine(program);
//    MatchStatus status = machine.Run("abc");
class Machine {
 public:
  Machine(const Program &program);

  // Set flags for positional match.
  Machine &SetMatchBegin(bool b);
  Machine &SetMatchEnd(bool b);

  // Run program on input string s with Rob Pike's implementation.
  // It maintains a collection of threads ready to run, and threads run in lock
  // step -- they process the same character in each iteration.
  // If capture is true, then capture groups will be saved.
  MatchStatus Run(const string &s, bool capture = true) const;

 private:
  friend class Thread;

  // Add a thread to run in this iteration.
  void AddReadyThread(ThreadPtr tp) const { ready.push(tp); }

  // Update match status.
  void UpdateStatus(const Thread::Status &tstatus) const;

  // Fetch instruction by program counter(index).
  const InstrPtr FetchInstruction(int pc) const { return program[pc]; }

 private:
  const Program program;
  mutable std::queue<ThreadPtr> ready;  // threads to run in current iteration
  mutable MatchStatus status;           // match status
  bool match_begin, match_end;          // flags for positonal match
};

};  // namespace Azuki

#endif  // __AZUKI_MACHINE__
