#include <cctype>
#include <iostream>
#include "machine.h"

namespace Azuki {

namespace {

void PopulateMatchResult(const Thread::Status &ts, MatchResult &ms) {
  ms.success = true;
  ms.begin = ts.begin;
  ms.end = ts.end;
  vector<string> temp;
  ;
  for (unsigned i = 0; i < ts.saved.size(); i += 2)
    temp.push_back(string(ts.saved[i], ts.saved[i + 1]));
  ms.capture = std::move(temp);
}

};  // namespace

MatchResult::MatchResult() : success(false), begin(0), end(0) {}

Thread::Thread(const Machine &machine, int pc, unsigned int begin)
    : machine(machine), pc(pc) {
  status.begin = begin;
  status.end = begin;
}

Thread *Thread::Split(int other_pc) {
  Thread *tp = new Thread(*this);
  tp->pc = other_pc;
  return tp;
}

bool Thread::RunOneStep(StringPtr sp, bool save_capture) {
  InstrPtr instr = machine.FetchInstruction(pc++);
  Opcode opcode = instr->opcode;

  if (instr->ConsumeCharacter()) ++status.end;

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
    if (status.repeated[instr->rpctr_idx] >= instr->low_times &&
        status.repeated[instr->rpctr_idx] <= instr->high_times)
      machine.AddReadyThread(shared_from_this());
  } else if (opcode == INCR) {
    ++status.repeated[instr->rpctr_idx];
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == JMP) {
    this->pc = instr->dst;
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == MATCH) {
    machine.UpdateResult(status);
  } else if (opcode == RANGE) {
    return (*sp) >= instr->low_ch && (*sp) <= instr->high_ch;
  } else if (opcode == SAVE) {
    if (save_capture) {
      if (status.saved.size() <= instr->save_idx)
        status.saved.resize(instr->save_idx + 1);
      status.saved[instr->save_idx] = sp;
    }
    machine.AddReadyThread(shared_from_this());
  } else if (opcode == SET) {
    if (status.repeated.size() <= instr->rpctr_idx)
      status.repeated.resize(instr->rpctr_idx + 1);
    status.repeated[instr->rpctr_idx] = instr->value;
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

MatchResult Machine::Run(const string &s, bool save_capture) const {
  ready = std::queue<ThreadPtr>();
  result.success = false;

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
      if (tp->RunOneStep(sp, save_capture)) next.push(tp);
      if (result.success) {
        if (match_end && idx != s.size()) {
          result.success = false;
        }
      }
    }
    ready = std::move(next);
  }
  return result;
}

void Machine::UpdateResult(const Thread::Status &ts) const {
  if (!result.success) {
    PopulateMatchResult(ts, result);
  } else {
    if (result.begin < ts.begin) {
      return;
    } else if (result.begin > ts.begin) {
      PopulateMatchResult(ts, result);
      return;
    } else {
      if (result.end < ts.end) PopulateMatchResult(ts, result);
    }
  }
}

};  // namespace Azuki
