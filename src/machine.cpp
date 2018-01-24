#include <iostream>
#include "machine.h"

namespace azuki {

bool Thread::RunOneStep(std::string::const_iterator sp) {
  InstrPtr instr = machine.FetchInstruction(pc++);

#if DEBUG
  std::cout << instr.str() << std::endl;
#endif

  switch (instr->opcode) {
    case ANY:
      return true;
    case CHAR:
      return (instr->c == *sp);
    case JMP:
      machine.AddReadyThread(Thread(machine, instr->dst, saved));
      break;
    case MATCH:
      machine.UpdateStatus(true);
      break;
    case SAVE:
      if (saved.size() <= instr->slot)
        saved.resize(instr->slot + 1);
      saved[instr->slot] = sp;
      machine.AddReadyThread(Thread(machine, pc, saved));
      break;
    case SPLIT:
      machine.AddReadyThread(Thread(machine, pc, saved));
      machine.AddReadyThread(Thread(machine, instr->dst, saved));
      break;
    default:
      std::cerr << "Error: invalid instruction type." << std::endl;
      throw std::exception();
  }
  return false;
}

Machine::Machine(const Program &program)
    : program(program), match_begin(false), match_end(false) {}

MatchStatus Machine::Run(const std::string &s) const {
  ready = std::queue<Thread>();
  status.match = false;

  if (match_begin) ready.push(Thread(*this, 0));

  // Need an extra character to finish ready threads.
  for (int i = 0; i <= s.size(); ++i) {
    auto sp = s.begin() + i;
    if (!match_begin) ready.push(Thread(*this, 0));

    std::queue<Thread> next;  // keep threads to run in next round
    while (!ready.empty()) {
      Thread t = ready.front();
      ready.pop();

      // If the thread successfully consumes the character, we need to save it
      // for next round.
      if (t.RunOneStep(sp)) next.push(t);
      if (status.match) {
        if (match_end && i != s.size()) {
          status.match = false;
        } else {
          status.saved = t.GetSaved();
          return status;
        }
      }
    }
    ready = std::move(next);
  }
  return status;
}

};  // namespace azuki
