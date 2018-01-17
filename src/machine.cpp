#include "machine.h"

#ifdef DEBUG
#include <iostream>
#endif

namespace azuki {

void Thread::ExecuteOneStep(char c) {
  InstrPtr instr = machine.FetchInstruction(pc);

#if DEBUG
  PrintInstruction(instr, pc);
  std::cout << c << std::endl;
#endif

  switch (instr->opcode) {
    case CHAR:
      if (instr->c == c) machine.AddThread(Thread(machine, pc + 1));
      break;
    case JMP:
      machine.AddThread(Thread(machine, instr->dst));
      break;
    case MATCH:
      machine.SetMatched(true);
      break;
    case SPLIT:
      machine.AddThread(Thread(machine, pc + 1));
      machine.AddThread(Thread(machine, instr->dst));
      break;
  }
}

bool Machine::Execute(const std::string &s) {
  ready = std::queue<Thread>();
  matched = false;

  for (char c : s + "$") {
    ready.push(Thread(*this, 0));
    std::queue<Thread> next;

    while (!ready.empty()) {
      Thread t = ready.front();
      ready.pop();
      t.ExecuteOneStep(c);
      if (matched) return true;
    }
    ready = std::move(next);
  }

  return matched;
}

};  // namespace azuki
