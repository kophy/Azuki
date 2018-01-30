#include <cctype>
#include <iostream>
#include "machine.h"

namespace Azuki {

namespace {

bool ConsumeCharacter(Opcode opcode) {
  return opcode == ANY || opcode == ANY_WORD || opcode == ANY_DIGIT ||
         opcode == ANY_SPACE || opcode == CHAR || opcode == RANGE;
}

};  // namespace

Thread::Thread(const Machine &machine, int pc, unsigned int begin_idx)
    : machine(machine), pc(pc) {
  status.begin_idx = begin_idx;
  status.end_idx = begin_idx;
}

ThreadPtr Thread::Split(int other_pc) {
  ThreadPtr tp(new Thread(*this));
  tp->pc = other_pc;
  return tp;
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
  } else if (opcode == CHECK) {
    if (status.repeated[instr->counter] >= instr->low_times &&
        status.repeated[instr->counter] <= instr->high_times)
      machine.AddReadyThread(shared_from_this());
  } else if (opcode == INCR) {
    ++status.repeated[instr->counter];
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == JMP) {
    machine.AddReadyThread(this->Split(instr->dst));
  } else if (opcode == MATCH) {
    status.success = true;
    machine.UpdateStatus(status);
  } else if (opcode == RANGE) {
    return (*sp) >= instr->low_ch && (*sp) <= instr->high_ch;
  } else if (opcode == SAVE) {
    if (capture) {
      if (status.saved.size() <= instr->slot)
        status.saved.resize(instr->slot + 1);
      status.saved[instr->slot] = sp;
    }
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == SET) {
    if (status.repeated.size() <= instr->counter)
      status.repeated.resize(instr->counter + 1);
    status.repeated[instr->counter] = instr->value;
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == SPLIT) {
    if (instr->greedy) {
      machine.AddReadyThread(this->Split(instr->dst));
      machine.AddReadyThread(shared_from_this());
    } else {
      machine.AddReadyThread(shared_from_this());
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
  ready = std::queue<ThreadPtr>();
  status.success = false;

  if (match_begin) ready.push(ThreadPtr(new Thread(*this, 0, 0)));

  // Need an extra character to finish ready threads.
  for (unsigned int idx = 0; idx <= s.size(); ++idx) {
    auto sp = s.begin() + idx;
    if (!match_begin) ready.push(ThreadPtr(new Thread(*this, 0, idx)));

    std::queue<ThreadPtr> next;  // keep threads to run in next round
    while (!ready.empty()) {
      ThreadPtr tp = ready.front();
      ready.pop();

      // If the thread successfully consumes the character, we need to save it
      // for next round.
      if (tp->RunOneStep(sp, capture)) next.push(tp);
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
