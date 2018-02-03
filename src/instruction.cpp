#include <iostream>
#include <sstream>
#include "instruction.h"

namespace Azuki {

namespace {

// The Context struct holds variables when compile the program recursively.
// It should be used only in this cpp file by CompileRegexp and Emit.
struct Context {
  int pc;
  unsigned int save_idx;
  unsigned int rpctr_idx;
  Context(int pc, int save_idx, int rpctr_idx)
      : pc(pc), save_idx(save_idx), rpctr_idx(rpctr_idx) {}
};

};  // namespace

// Calculate the number of instructions required to represent the regexp.
int CalculateInstruction(RegexpPtr rp);

// Emit instructions compiled from regexp to program with starting index pc.
void Emit(Program &program, Context &context, RegexpPtr rp);

bool Instruction::ConsumeCharacter() {
  return opcode == ANY || opcode == ANY_WORD || opcode == ANY_DIGIT ||
         opcode == ANY_SPACE || opcode == CHAR || opcode == RANGE;
}

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
    case CHECK:
      ss << "CHECK rpctr_idx[" << rpctr_idx << "] " << low_times << " "
         << high_times;
      break;
    case INCR:
      ss << "INCR rpctr_idx[" << rpctr_idx << "]";
      break;
    case JMP:
      ss << "JMP I" << dst;
      break;
    case MATCH:
      ss << "MATCH";
      break;
    case SAVE:
      ss << "SAVE " << save_idx;
      break;
    case SET:
      ss << "SET rpctr_idx[" << rpctr_idx << "]";
      break;
    case SPLIT:
      ss << "SPLIT I" << idx + 1 << " I" << dst;
      break;
    case RANGE:
      ss << "RANGE " << low_ch << " " << high_ch;
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

InstrPtr CreateSaveInstruction(unsigned int save_idx) {
  InstrPtr rp(new Instruction(SAVE));
  rp->save_idx = save_idx;
  return rp;
}

InstrPtr CreateSplitInstruction(unsigned int dst, bool greedy) {
  InstrPtr instr(new Instruction(SPLIT));
  instr->dst = dst;
  instr->greedy = greedy;
  return instr;
}

InstrPtr CreateJmpInstruction(unsigned int dst) {
  InstrPtr instr(new Instruction(JMP));
  instr->dst = dst;
  return instr;
}

InstrPtr CreateRangeInstruction(char low_ch, char high_ch) {
  InstrPtr instr(new Instruction(RANGE));
  instr->low_ch = low_ch;
  instr->high_ch = high_ch;
  return instr;
}

InstrPtr CreateCheckInstruction(unsigned int rpctr_idx, int low_times,
                                int high_times) {
  InstrPtr instr(new Instruction(CHECK));
  instr->rpctr_idx = rpctr_idx;
  instr->low_times = low_times;
  instr->high_times = high_times;
  return instr;
}

InstrPtr CreateIncrInstruction(unsigned int rpctr_idx) {
  InstrPtr instr(new Instruction(INCR));
  instr->rpctr_idx = rpctr_idx;
  return instr;
}

InstrPtr CreateSetInstruction(unsigned int rpctr_idx, int value) {
  InstrPtr instr(new Instruction(SET));
  instr->rpctr_idx = rpctr_idx;
  instr->value = value;
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
    case CURLY:
      return 5 + CalculateInstructionImpl(rp->left);
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
    case SQUARE:
      return 1;
    default:
      throw std::runtime_error("Unexpected regexp type.");
  }
}

void Emit(Program &program, Context &context, RegexpPtr rp) {
  int &pc = context.pc;
  unsigned int &save_idx = context.save_idx;
  unsigned int &rpctr_idx = context.rpctr_idx;

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
  } else if (rp->type == CURLY) {
    int set_pc = pc++;
    int old_rpctr_idx = rpctr_idx++;
    program[set_pc] = CreateSetInstruction(old_rpctr_idx, 0);
    Emit(program, context, rp->left);
    program[pc++] = CreateIncrInstruction(old_rpctr_idx);
    int split_pc = pc++;
    int jmp_pc = pc++;
    int check_pc = pc++;
    program[split_pc] = CreateSplitInstruction(check_pc, false);
    program[jmp_pc] = CreateJmpInstruction(set_pc + 1);
    program[check_pc] =
        CreateCheckInstruction(old_rpctr_idx, rp->low_times, rp->high_times);
  } else if (rp->type == DOT) {
    program[pc++] = CreateAnyInstruction();
  } else if (rp->type == LIT) {
    program[pc++] = CreateCharInstruction(rp->c);
  } else if (rp->type == PAREN) {
    int old_save_idx = save_idx;
    save_idx += 2;
    program[pc++] = CreateSaveInstruction(old_save_idx);
    Emit(program, context, rp->left);
    program[pc++] = CreateSaveInstruction(old_save_idx + 1);
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
  } else if (rp->type == SQUARE) {
    program[pc++] = CreateRangeInstruction(rp->low_ch, rp->high_ch);
  } else {
    throw std::runtime_error("Unexpected regexp type.");
  }
}

Program CompileRegexp(RegexpPtr rp) {
  Program program(CalculateInstruction(rp));
  Context context(0, 0, 0);
  Emit(program, context, rp);
  program.back() = CreateMatchInstruction();

  for (unsigned int idx = 0; idx < program.size(); ++idx)
    program[idx]->idx = idx;
  return program;
}

void PrintProgram(const Program &program) {
  for (int idx = 0; idx < program.size(); ++idx) {
    auto &instr = program[idx];
    std::cout << instr->str() << std::endl;
  }
}

};  // namespace Azuki
