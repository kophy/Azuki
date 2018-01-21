#include "machine.h"
#include <iostream>

namespace azuki {

bool Thread::RunOneStep(char c) {
  InstrPtr instr = machine.FetchInstruction(pc++);

#if DEBUG
  PrintInstruction(instr, pc);
  std::cout << c << std::endl;
#endif

  switch (instr->opcode) {
    case ANY:
      return true;
    case CHAR:
      return (instr->c == c);
    case DOT:
      return true;
    case JMP:
      machine.AddReadyThread(Thread(machine, instr->dst));
      break;
    case MATCH:
      machine.UpdateStatus(true);
      break;
    case SPLIT:
      machine.AddReadyThread(Thread(machine, pc));
      machine.AddReadyThread(Thread(machine, instr->dst));
      break;
    default:
      // TODO: add error handling code
      std::cerr << "Error: invalid instruction type." << std::endl;
      throw std::exception();
  }
  return false;
}

MatchStatus Machine::Run(const std::string &s) {
  ready = std::queue<Thread>();
  ready.push(Thread(*this, 0));

  status.match = false;

  // Need an extra character '$' to finish ready threads.
  for (char c : s + "$") {
    std::queue<Thread> next;

    while (!ready.empty()) {
      Thread t = ready.front();
      ready.pop();

      // If the thread successfully consumes the character, we need to save it
      // for next round.
      if (t.RunOneStep(c)) next.push(t);
      if (status.match) return status;
    }
    ready = std::move(next);
  }
  return status;
}

};  // namespace azuki
