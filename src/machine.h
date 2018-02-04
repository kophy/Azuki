#ifndef __AZUKI_MACHINE__
#define __AZUKI_MACHINE__

#include <queue>
#include "common.h"
#include "instruction.h"

namespace Azuki {

// The MatchResult struct holds results of regexp match.
struct MatchResult {
  bool success;
  unsigned int begin, end;  // begin and end index of matched substring
  vector<string> capture;   // capture groups

  MatchResult();
};

class Machine;  // forward declaration

// The Thread class implements "fake" threads to run in the virtual machine.
// Each thread keeps its own program counter and match status.
class Thread : public std::enable_shared_from_this<Thread> {
 public:
  // The Thread::Status struct holds current match status of a thread.
  struct Status {
    unsigned int begin, end;  // begin and end index of current substring
    vector<StringPtr> saved;  // begin and end pointer of capture groups
    vector<int> repeated;     // counters of repeat times
  };

 public:
  Thread(const Machine &machine, int pc, unsigned int begin);

  // Create a new thread with exact same state as this thread but a different
  // program counter.
  Thread *Split(int other_pc);

  // Return true if the thread successfully consumes the input character
  // referenced by sp. The thread should be run in next iteration. Otherwise,
  // the thread either runs a control instruction and succeeds, or runs a data
  // instruction and fails.
  // If save_capture is true, then capture groups will be saved.
  bool RunOneStep(StringPtr sp, bool save_capture = true);

 private:
  const Machine &machine;  // host machine
  unsigned int pc;         // program counter
  Thread::Status status;   // this thread's match status
};

typedef shared_ptr<Thread> ThreadPtr;

// The Machine class implements a virtual machine to run Thompson's algorithm.
// Example:
//    Machine machine(program);
//    MatchResult status = machine.Run("abc");
class Machine {
 public:
  Machine(const Program &program);

  // Set flags for positional match.
  void SetMatchBegin(bool b) { match_begin = b; }
  void SetMatchEnd(bool b)  { match_end = b; }

  // Run program on input string s with Rob Pike's implementation.
  // It maintains a collection of threads ready to run, and threads run in lock
  // step -- all threads process the same character in each iteration.
  // If save_capture is true, then capture groups will be saved.
  MatchResult Run(const string &s, bool save_capture = true) const;

 private:
  friend class Thread;

  // Add a thread to run in current iteration.
  void AddReadyThread(ThreadPtr tp) const { ready.push(tp); }

  // Update match result (called only when the thread successfully matches).
  void UpdateResult(const Thread::Status &tstatus) const;

  // Fetch instruction by program counter (index).
  const InstrPtr FetchInstruction(int pc) const { return program[pc]; }

 private:
  const Program program;
  mutable std::queue<ThreadPtr> ready;  // threads to run in current iteration
  mutable MatchResult result;           // match result
  bool match_begin, match_end;          // flags for positonal match
};

};  // namespace Azuki

#endif  // __AZUKI_MACHINE__
