#include <iostream>
#include <sstream>
#include "instruction.h"

namespace Azuki {

// Count the number of instructions required to represent the regexp.
int CountInstruction(RegexpPtr r);

// Emit instructions compiled from regexp to program with starting index pc.
void Emit(Program &program, int &pc, int &slot, RegexpPtr rp);

std::string Instruction::str() {
  std::stringstream ss;
  ss << "I" << idx << " ";
  switch (opcode) {
    case ANY:
      ss << "ANY";
      break;
    case ANY_WORD:
      ss << "ANY WORD";
      break;
    case ANY_DIGIT:
      ss << "ANY DIGIT";
      break;
    case ANY_SPACE:
      ss << "ANY SPACE";
      break;
    case CHAR:
      ss << "CHAR '" << c << "'";
      break;
    case JMP:
      ss << "JMP I" << dst;
      break;
    case MATCH:
      ss << "MATCH";
      break;
    case SAVE:
      ss << "SAVE " << slot;
      break;
    case SPLIT:
      ss << "SPLIT I" << idx + 1 << " I" << dst;
      break;
    default:
      throw std::runtime_error("Unexpected instruction opcode.");
  }
  return ss.str();
}

InstrPtr CreateAnyInstruction() { return InstrPtr(new Instruction(ANY)); }

InstrPtr CreateAnyWordInstruction() {
  return InstrPtr(new Instruction(ANY_WORD));
}

InstrPtr CreateAnyDigitInstruction() {
  return InstrPtr(new Instruction(ANY_DIGIT));
}

InstrPtr CreateAnySpaceInstruction() {
  return InstrPtr(new Instruction(ANY_SPACE));
}

InstrPtr CreateCharInstruction(char c) {
  InstrPtr instr(new Instruction(CHAR));
  instr->c = c;
  return InstrPtr(instr);
}

InstrPtr CreateMatchInstruction() { return InstrPtr(new Instruction(MATCH)); }

InstrPtr CreateSaveInstruction(int slot) {
  InstrPtr rp(new Instruction(SAVE));
  rp->slot = slot;
  return rp;
}

InstrPtr CreateSplitInstruction(int dst, bool greedy) {
  InstrPtr instr(new Instruction(SPLIT));
  instr->dst = dst;
  instr->greedy = greedy;
  return instr;
}

InstrPtr CreateJmpInstruction(int dst) {
  InstrPtr instr(new Instruction(JMP));
  instr->dst = dst;
  return instr;
}

int CountInstructionImpl(RegexpPtr rp);
int CountInstruction(RegexpPtr rp) { return CountInstructionImpl(rp) + 1; }

int CountInstructionImpl(RegexpPtr rp) {
  switch (rp->type) {
    case ALT:
      return 2 + CountInstructionImpl(rp->left) +
             CountInstructionImpl(rp->right);
    case CAT:
      return CountInstructionImpl(rp->left) + CountInstructionImpl(rp->right);
    case CLASS:
      return 1;
    case DOT:
      return 1;
    case LIT:
      return 1;
    case PAREN:
      return 2 + CountInstructionImpl(rp->left);
    case PLUS:
      return 2 + CountInstructionImpl(rp->left);
    case QUEST:
      return 1 + CountInstructionImpl(rp->left);
    case STAR:
      return 2 + CountInstructionImpl(rp->left);
    default:
      throw std::runtime_error("Unexpected regexp type.");
  }
}

Program CompileRegexp(RegexpPtr rp) {
  Program program(CountInstruction(rp));
  int pc = 0, slot = 0;
  Emit(program, pc, slot, rp);
  program[pc] = CreateMatchInstruction();

  for (int idx = 0; idx < program.size(); ++idx) program[idx]->idx = idx;
  return program;
}

void Emit(Program &program, int &pc, int &slot, RegexpPtr rp) {
  if (rp->type == ALT) {
    int split_pc = pc++;
    Emit(program, pc, slot, rp->left);
    program[split_pc] = CreateSplitInstruction(pc + 1);
    int jmp_pc = pc++;
    Emit(program, pc, slot, rp->right);
    program[jmp_pc] = CreateJmpInstruction(pc);
  } else if (rp->type == CAT) {
    Emit(program, pc, slot, rp->left);
    Emit(program, pc, slot, rp->right);
  } else if (rp->type == CLASS) {
    if (rp->c == 'w')
      program[pc++] = CreateAnyWordInstruction();
    else if (rp->c == 'd')
      program[pc++] = CreateAnyDigitInstruction();
    else
      program[pc++] = CreateAnySpaceInstruction();
  } else if (rp->type == DOT) {
    program[pc++] = CreateAnyInstruction();
  } else if (rp->type == LIT) {
    program[pc++] = CreateCharInstruction(rp->c);
  } else if (rp->type == PAREN) {
    int old_slot = slot;
    slot += 2;
    program[pc++] = CreateSaveInstruction(old_slot);
    Emit(program, pc, slot, rp->left);
    program[pc++] = CreateSaveInstruction(old_slot + 1);
  } else if (rp->type == PLUS) {
    int current_pc = pc;
    Emit(program, pc, slot, rp->left);
    program[pc] = CreateSplitInstruction(pc + 2, true);
    ++pc;
    program[pc++] = CreateJmpInstruction(current_pc);
  } else if (rp->type == QUEST) {
    int split_pc = pc++;
    Emit(program, pc, slot, rp->left);
    program[split_pc] = CreateSplitInstruction(pc);
  } else if (rp->type == STAR) {
    int split_pc = pc++;
    Emit(program, pc, slot, rp->left);
    program[pc++] = CreateJmpInstruction(split_pc);
    program[split_pc] = CreateSplitInstruction(pc, true);
  } else {
    throw std::runtime_error("Unexpected regexp type.");
  }
}

void PrintProgram(const Program &program) {
  for (int idx = 0; idx < program.size(); ++idx) {
    auto &instr = program[idx];
    std::cout << instr->str() << std::endl;
  }
}

};  // namespace Azuki
