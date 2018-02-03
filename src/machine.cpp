#include <cctype>
#include <iostream>
#include "machine.h"

namespace Azuki {

namespace {

void PopulateMatchStatus(const Thread::Status &ts, MatchStatus &ms) {
  ms.success = true;
  ms.begin = ts.begin;
  ms.end = ts.end;
  vector<string> temp;;
  for (unsigned i = 0; i < ts.saved.size(); i += 2)
    temp.push_back(string(ts.saved[i], ts.saved[i + 1]));
  ms.captured = std::move(temp);
}

};  // namespace

MatchStatus::MatchStatus() : success(false), begin(0), end(0) {}

Thread::Thread(const Machine &machine, int pc, unsigned int begin)
    : machine(machine), pc(pc) {
  tstatus.begin = begin;
  tstatus.end = begin;
}

Thread *Thread::Split(int other_pc) {
  Thread *tp = new Thread(*this);
  tp->pc = other_pc;
  return tp;
}

bool Thread::RunOneStep(StringPtr sp, bool capture) {
  InstrPtr instr = machine.FetchInstruction(pc++);
  Opcode opcode = instr->opcode;

  if (instr->ConsumeCharacter()) ++tstatus.end;

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
    if (tstatus.repeated[instr->rpctr_idx] >= instr->low_times &&
        tstatus.repeated[instr->rpctr_idx] <= instr->high_times)
      machine.AddReadyThread(shared_from_this());
  } else if (opcode == INCR) {
    ++tstatus.repeated[instr->rpctr_idx];
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == JMP) {
    this->pc = instr->dst;
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == MATCH) {
    machine.UpdateStatus(tstatus);
  } else if (opcode == RANGE) {
    return (*sp) >= instr->low_ch && (*sp) <= instr->high_ch;
  } else if (opcode == SAVE) {
    if (capture) {
      if (tstatus.saved.size() <= instr->save_idx)
        tstatus.saved.resize(instr->save_idx + 1);
      tstatus.saved[instr->save_idx] = sp;
    }
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == SET) {
    if (tstatus.repeated.size() <= instr->rpctr_idx)
      tstatus.repeated.resize(instr->rpctr_idx + 1);
    tstatus.repeated[instr->rpctr_idx] = instr->value;
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == SPLIT) {
    if (instr->greedy) {
      machine.AddReadyThread(ThreadPtr(this->Split(instr->dst)));
      machine.AddReadyThread(shared_from_this());
    } else {
      machine.AddReadyThread(shared_from_this());
      machine.AddReadyThread(ThreadPtr(this->Split(instr->dst)));
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
      auto tp = ready.front();
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

void Machine::UpdateStatus(const Thread::Status &ts) const {
  if (!status.success) {
    PopulateMatchStatus(ts, status);
  } else {
    if (status.begin < ts.begin) {
      return;
    } else if (status.begin > ts.begin) {
      PopulateMatchStatus(ts, status);
      return;
    } else {
      if (status.end < ts.end) PopulateMatchStatus(ts, status);
    }
  }
}

};  // namespace Azuki
