#include <cctype>
#include <iostream>
#include "machine.h"

namespace Azuki {

namespace {

bool ConsumeCharacter(Opcode opcode) {
  return opcode == ANY || opcode == ANY_WORD || opcode == ANY_DIGIT ||
         opcode == ANY_SPACE || opcode == CHAR;
}

};  // namespace

Thread::Thread(const Machine &machine, int pc, unsigned int begin_idx)
    : machine(machine), pc(pc) {
  status.begin_idx = begin_idx;
  status.end_idx = begin_idx;
}

Thread Thread::Split(int new_pc) {
  Thread t(*this);
  t.pc = new_pc;
  return t;
}

bool Thread::RunOneStep(StringPtr sp, bool capture) {
  InstrPtr instr = machine.FetchInstruction(pc++);
  Opcode opcode = instr->opcode;

  if (ConsumeCharacter(opcode)) ++status.end_idx;

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
    status.success = true;
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

MatchStatus Machine::Run(const string &s, bool capture) const {
  ready = std::queue<Thread>();
  status.success = false;

  if (match_begin) ready.push(Thread(*this, 0, 0));

  // Need an extra character to finish ready threads.
  for (unsigned int idx = 0; idx <= s.size(); ++idx) {
    auto sp = s.begin() + idx;
    if (!match_begin) ready.push(Thread(*this, 0, idx));

    std::queue<Thread> next;  // keep threads to run in next round
    while (!ready.empty()) {
      Thread t = ready.front();
      ready.pop();

      // If the thread successfully consumes the character, we need to save it
      // for next round.
      if (t.RunOneStep(sp, capture)) next.push(t);
      if (status.success) {
        if (match_end && idx != s.size()) {
          status.success = false;
        }
      }
    }
    ready = std::move(next);
  }
  return status;
}

// TODO: this part should be tested
void Machine::UpdateStatus(const MatchStatus &status_) const {
  if (!status.success) {
    status = status_;
  } else {
    if (status.begin_idx < status_.begin_idx) {
      return;
    } else if (status.begin_idx > status_.begin_idx) {
      status = status_;
      return;
    } else {
      if (status.end_idx < status_.end_idx) status = status_;
    }
  }
}

};  // namespace Azuki
