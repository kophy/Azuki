#include <iostream>
#include <sstream>
#include "instruction.h"

namespace azuki {

// Convenience functions to create different instructions.
InstrPtr CreateAnyInstruction();
InstrPtr CreateCharInstruction(char c);
InstrPtr CreateMatchInstruction();
InstrPtr CreateSplitInstruction(int dst);
InstrPtr CreateJmpInstruction(int dst);

// Count the number of instructions required to represent the regexp.
int CountInstruction(RegexpPtr r);

// Emit instructions compiled from regexp to program with starting index pc.
void Emit(Program &program, int &pc, RegexpPtr r);

std::string Instruction::str() {
  std::stringstream ss;
  ss << "I" << idx << " ";
  switch (opcode) {
    case ANY:
      ss << "ANY";
      break;
    case CHAR:
      ss << "CHAR " << c;
      break;
    case JMP:
      ss << "JMP I" << dst;
      break;
    case MATCH:
      ss << "MATCH";
      break;
    case SPLIT:
      ss << "SPLIT I" << idx + 1 << " I" << dst;
      break;
    default:
      std::cerr << "Error: invalid instruction type." << std::endl;
      throw std::exception();
  }
  return ss.str();
}

InstrPtr CreateAnyInstruction() { return InstrPtr(new Instruction(ANY)); }

InstrPtr CreateCharInstruction(char c) {
  InstrPtr instr(new Instruction(CHAR));
  instr->c = c;
  return InstrPtr(instr);
}

InstrPtr CreateMatchInstruction() { return InstrPtr(new Instruction(MATCH)); }

InstrPtr CreateSplitInstruction(int dst) {
  InstrPtr instr(new Instruction(SPLIT));
  instr->dst = dst;
  return instr;
}

InstrPtr CreateJmpInstruction(int dst) {
  InstrPtr instr(new Instruction(JMP));
  instr->dst = dst;
  return instr;
}

int CountInstructionImpl(RegexpPtr r);
int CountInstruction(RegexpPtr r) { return CountInstructionImpl(r) + 1; }

int CountInstructionImpl(RegexpPtr r) {
  switch (r->type) {
    case ALT:
      return 2 + CountInstructionImpl(r->left) + CountInstructionImpl(r->right);
    case CAT:
      return CountInstructionImpl(r->left) + CountInstructionImpl(r->right);
    case DOT:
      return 1;
    case LIT:
      return 1;
    case PLUS:
      return 2 + CountInstructionImpl(r->left);
    case QUEST:
      return 1 + CountInstructionImpl(r->left);
    case STAR:
      return 2 + CountInstructionImpl(r->left);
    default:
      std::cerr << "Error: invalid regexp type." << std::endl;
      throw std::exception();
  }
}

Program CompileRegex(RegexpPtr r) {
  Program program(CountInstruction(r));
  int pc = 0;
  Emit(program, pc, r);
  program[pc] = CreateMatchInstruction();

  for (int idx = 0; idx < program.size(); ++idx) program[idx]->idx = idx;
  return program;
}

void Emit(Program &program, int &pc, RegexpPtr r) {
  if (r->type == ALT) {
    int split_pc = pc++;
    Emit(program, pc, r->left);
    program[split_pc] = CreateSplitInstruction(pc + 1);
    int jmp_pc = pc++;
    Emit(program, pc, r->right);
    program[jmp_pc] = CreateJmpInstruction(pc);
  } else if (r->type == CAT) {
    Emit(program, pc, r->left);
    Emit(program, pc, r->right);
  } else if (r->type == DOT) {
    program[pc++] = CreateAnyInstruction();
  } else if (r->type == LIT) {
    program[pc++] = CreateCharInstruction(r->c);
  } else if (r->type == PLUS) {
    int current_pc = pc;
    Emit(program, pc, r->left);
    program[pc] = CreateSplitInstruction(pc + 2);
    ++pc;
    program[pc++] = CreateJmpInstruction(current_pc);
  } else if (r->type == QUEST) {
    int split_pc = pc++;
    Emit(program, pc, r->left);
    program[split_pc] = CreateSplitInstruction(pc);
  } else if (r->type == STAR) {
    int split_pc = pc++;
    Emit(program, pc, r->left);
    program[pc++] = CreateJmpInstruction(split_pc);
    program[split_pc] = CreateSplitInstruction(pc);
  } else {
    std::cerr << "Error: invalid regexp type." << std::endl;
    throw std::exception();
  }
}

void PrintProgram(const Program &program) {
  for (int idx = 0; idx < program.size(); ++idx) {
    auto &instr = program[idx];
    std::cout << instr->str() << std::endl;
  }
}

};  // namespace azuki
