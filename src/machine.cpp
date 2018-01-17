#include "machine.h"

#ifdef DEBUG
#include <iostream>
#endif

namespace azuki {

bool Thread::RunOneStep(char c) {
  InstrPtr instr = machine.FetchInstruction(pc);

#if DEBUG
  PrintInstruction(instr, pc);
  std::cout << c << std::endl;
#endif

  switch (instr->opcode) {
    case ANY:
      return true;
    case CHAR:
      ++pc;
      return (instr->c == c);
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
    default:
      // TODO: add error handling code
      break;
  }
  return false;
}

bool Machine::Run(const std::string &s) {
  ready = std::queue<Thread>();
  matched = false;

  for (char c : s + "$") {
    ready.push(Thread(*this, 0));
    std::queue<Thread> next;

    while (!ready.empty()) {
      Thread t = ready.front();
      ready.pop();

      // If the thread successfully consumes the character, we need to save it
      // for next round.
      if (t.RunOneStep(c)) next.push(t);
      if (matched) return true;
    }
    ready = std::move(next);
  }

  return matched;
}

};  // namespace azuki
