#include <iostream>
#include <sstream>
#include "instruction.h"

namespace azuki {

// Helper function to count the number of instructions.
int count(std::shared_ptr<Regexp> r);

// Helper function to compile regex into program.
void Emit(Program &program, int &pc, std::shared_ptr<Regexp> r);

std::string Instruction::to_string() {
  std::stringstream ss;
  ss << "I" << idx << " ";
  switch (opcode) {
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

int CountInstruction(std::shared_ptr<RegexNode> r) { return count(r) + 1; }

int count(std::shared_ptr<Regexp> r) {
  switch (r->type) {
    case ALT:
      return 2 + count(r->left) + count(r->right);
    case CAT:
      return count(r->left) + count(r->right);
    case LIT:
      return 1;
    default:
      std::cerr << "Error: invalid regexp type." << std::endl;
      throw std::exception();
  }
}

Program CompileRegex(std::shared_ptr<RegexNode> r) {
  Program program(CountInstruction(r));
  int pc = 0;
  Emit(program, pc, r);
  program[pc] = CreateMatchInstruction();

  for (int idx = 0; idx < program.size(); ++idx)
    program[idx]->idx = idx;
  return program;
}

void Emit(Program &program, int &pc, std::shared_ptr<Regexp> r) {
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
  } else if (r->type == LIT) {
    program[pc++] = CreateCharInstruction(r->c);
  } else {
    std::cerr << "Error: invalid regexp type." << std::endl;
    throw std::exception();
  }
}

void PrintProgram(Program &program) {
  for (int idx = 0; idx < program.size(); ++idx) {
    auto &instr = program[idx];
    std::cout << instr->to_string() << std::endl;
  }
}

};  // namespace azuki
