#include <iostream>
#include <sstream>
#include "instruction.h"

namespace Azuki {

namespace {

// The Context struct holds variables when compile the program recursively.
// It should be used only in this cpp file by CompileRegexp and Emit, so here an
// anonymous namespace is used.
struct Context {
  int pc;
  int slot;
  Context(int pc, int slot) : pc(pc), slot(slot) {}
};

};  // namespace

// Calculate the number of instructions required to represent the regexp.
int CalculateInstruction(RegexpPtr rp);

// Emit instructions compiled from regexp to program with starting index pc.
void Emit(Program &program, Context &context, RegexpPtr rp);

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

int CalculateInstructionImpl(RegexpPtr rp);
int CalculateInstruction(RegexpPtr rp) {
  return CalculateInstructionImpl(rp) + 1;  // the last instruction "MATCH"
}

int CalculateInstructionImpl(RegexpPtr rp) {
  switch (rp->type) {
    case ALT:
      return 2 + CalculateInstructionImpl(rp->left) +
             CalculateInstructionImpl(rp->right);
    case CAT:
      return CalculateInstructionImpl(rp->left) +
             CalculateInstructionImpl(rp->right);
    case CLASS:
      return 1;
    case DOT:
      return 1;
    case LIT:
      return 1;
    case PAREN:
      return 2 + CalculateInstructionImpl(rp->left);
    case PLUS:
      return 2 + CalculateInstructionImpl(rp->left);
    case QUEST:
      return 1 + CalculateInstructionImpl(rp->left);
    case STAR:
      return 2 + CalculateInstructionImpl(rp->left);
    default:
      throw std::runtime_error("Unexpected regexp type.");
  }
}

// Emit instructions compiled from regexp to program with starting index pc.
void Emit(Program &program, Context &context, RegexpPtr rp) {
  int &pc = context.pc;
  int &slot = context.slot;
  
  if (rp->type == ALT) {
    int split_pc = pc++;
    Emit(program, context, rp->left);
    program[split_pc] = CreateSplitInstruction(pc + 1);
    int jmp_pc = pc++;
    Emit(program, context, rp->right);
    program[jmp_pc] = CreateJmpInstruction(pc);
  } else if (rp->type == CAT) {
    Emit(program, context, rp->left);
    Emit(program, context, rp->right);
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
    Emit(program, context, rp->left);
    program[pc++] = CreateSaveInstruction(old_slot + 1);
  } else if (rp->type == PLUS) {
    int current_pc = pc;
    Emit(program, context, rp->left);
    program[pc] = CreateSplitInstruction(pc + 2, true);
    ++pc;
    program[pc++] = CreateJmpInstruction(current_pc);
  } else if (rp->type == QUEST) {
    int split_pc = pc++;
    Emit(program, context, rp->left);
    program[split_pc] = CreateSplitInstruction(pc);
  } else if (rp->type == STAR) {
    int split_pc = pc++;
    Emit(program, context, rp->left);
    program[pc++] = CreateJmpInstruction(split_pc);
    program[split_pc] = CreateSplitInstruction(pc, true);
  } else {
    throw std::runtime_error("Unexpected regexp type.");
  }
}

Program CompileRegexp(RegexpPtr rp) {
  Program program(CalculateInstruction(rp));
  Context context(0, 0);
  Emit(program, context, rp);
  program.back() = CreateMatchInstruction();

  for (int idx = 0; idx < program.size(); ++idx) program[idx]->idx = idx;
  return program;
}

void PrintProgram(const Program &program) {
  for (int idx = 0; idx < program.size(); ++idx) {
    auto &instr = program[idx];
    std::cout << instr->str() << std::endl;
  }
}

};  // namespace Azuki
