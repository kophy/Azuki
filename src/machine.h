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
  Thread(Machine &machine_, int pc_) : machine(machine_), pc(pc_) {}

  // Return true if in this one step, this thread successfully consumes the
  // given character c.
  void ExecuteOneStep(char c);
 private:
  Machine &machine;
  int pc;
};

class Machine {
 public:
  Machine(const Program &program_) : program(program_) {}

  bool Execute(const std::string &s);

  void AddThread(Thread &&thread) { ready.push(thread); }

  void SetMatched(bool matched_) { matched = matched_; }

  const InstrPtr FetchInstruction(int pc) const { return program[pc]; }

 private:
  const Program program;

  std::queue<Thread> ready;

  bool matched;
};

};  // namespace azuki

#endif
