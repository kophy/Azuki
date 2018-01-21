#include <iostream>
#include "machine.h"

namespace azuki {

bool Thread::RunOneStep(char c) {
  InstrPtr instr = machine.FetchInstruction(pc++);

#if DEBUG
  std::cout << instr.str() << std::endl;
#endif

  switch (instr->opcode) {
    case ANY:
      return true;
    case CHAR:
      return (instr->c == c);
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
      std::cerr << "Error: invalid instruction type." << std::endl;
      throw std::exception();
  }
  return false;
}

Machine::Machine(const Program &program)
    : program(program), match_begin(false), match_end(false) {}

MatchStatus Machine::Run(const std::string &s) {
  ready = std::queue<Thread>();
  status.match = false;

  if (match_begin) ready.push(Thread(*this, 0));

  // Need an extra character to finish ready threads.
  for (char c : s + " ") {
    if (!match_begin) ready.push(Thread(*this, 0));

    std::queue<Thread> next;  // keep threads to run in next round
    while (!ready.empty()) {
      Thread t = ready.front();
      ready.pop();

      // If the thread successfully consumes the character, we need to save it
      // for next round.
      if (t.RunOneStep(c)) next.push(t);
      if (status.match) {
        if (match_end) {
          status.match = false;
        } else {
          return status;
        }
      }
    }
    ready = std::move(next);
  }
  return status;
}

};  // namespace azuki
