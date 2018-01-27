#include <cctype>
#include <iostream>
#include "machine.h"

namespace Azuki {

Thread::Thread(const Machine &machine, int pc) : machine(machine), pc(pc) {}

Thread Thread::Split(int new_pc) {
  Thread t(*this);
  t.pc = new_pc;
  return t;
}

bool Thread::RunOneStep(StringPtr sp, bool capture) {
  InstrPtr instr = machine.FetchInstruction(pc++);
  Opcode opcode = instr->opcode;

  if (opcode == ANY) {
    return true;
  } else if (opcode == ANY_WORD) {
    return isalnum(*sp) || (*sp == '_');
  } else if (opcode == ANY_DIGIT) {
    return isdigit(*sp);
  } else if (opcode == ANY_SPACE) {
    return isspace(*sp);
  } else if (opcode == CHAR) {
    return (instr->c == *sp);
  } else if (opcode == JMP) {
    machine.AddReadyThread(this->Split(instr->dst));
  } else if (opcode == MATCH) {
    status.match = true;
    machine.UpdateStatus(status);
  } else if (opcode == SAVE) {
    if (capture) {
      if (status.saved.size() <= instr->slot)
        status.saved.resize(instr->slot + 1);
      status.saved[instr->slot] = sp;
    }
    machine.AddReadyThread(this->Split(pc));
  } else if (opcode == SPLIT) {
    if (instr->greedy) {
      machine.AddReadyThread(this->Split(instr->dst));
      machine.AddReadyThread(this->Split(pc));
    } else {
      machine.AddReadyThread(this->Split(pc));
      machine.AddReadyThread(this->Split(instr->dst));
    }
  } else {
    throw std::runtime_error("Unexpected instruction opcode.");
  }
  return false;
}

MatchStatus::MatchStatus() {}

MatchStatus::MatchStatus(bool match, const vector<StringPtr> &saved)
    : match(match), saved(saved) {}

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
  for (unsigned int idx = 0; idx <= s.size(); ++idx) {
    auto sp = s.begin() + idx;
    if (!match_begin) ready.push(Thread(*this, 0));

    std::queue<Thread> next;  // keep threads to run in next round
    while (!ready.empty()) {
      Thread t = ready.front();
      ready.pop();

      // If the thread successfully consumes the character, we need to save it
      // for next round.
      if (t.RunOneStep(sp)) next.push(t);
      if (status.match) {
        if (match_end && idx != s.size()) {
          status.match = false;
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

};  // namespace Azuki
