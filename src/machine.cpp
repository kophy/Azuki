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
      machine.AddReadyThread(Thread(machine, instr->dst, status));
      break;
    case MATCH:
      status.match = true;
      machine.UpdateStatus(status);
      break;
    case SAVE:
      if (status.saved.size() <= instr->slot)
        status.saved.resize(instr->slot + 1);
      status.saved[instr->slot] = sp;
      machine.AddReadyThread(Thread(machine, pc, status));
      break;
    case SPLIT:
      machine.AddReadyThread(Thread(machine, pc, status));
      machine.AddReadyThread(Thread(machine, instr->dst, status));
      break;
    default:
      throw std::runtime_error("Unexpected instruction opcode.");
  }
  return false;
}

Machine::Machine(const Program &program)
    : program(program), match_begin(false), match_end(false) {}

Machine &Machine::SetMatchBegin(bool b) {
  match_begin = b;
  return *this;
}

Machine &Machine::SetMatchEnd(bool b) {
  match_end = b;
  return *this;
}

MatchStatus Machine::Run(const std::string &s, bool capture) const {
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
          return status;
        }
      }
    }
    ready = std::move(next);
  }
  return status;
}

void Machine::UpdateStatus(const MatchStatus &status_) const {
  status = status_;
}

};  // namespace azuki
