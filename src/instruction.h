#ifndef __AZUKI_INSTR__
#define __AZUKI_INSTR__

#include <memory>
#include <string>
#include <vector>
#include "regexp.h"

namespace azuki {

enum Opcode {
  ANY,
  CHAR,
  MATCH,
  NIL,
  SPLIT,
  JMP
};

struct Instruction {
  Opcode opcode;      // used in all instructions
  char c;             // only used in CHAR instruction
  int dst;            // only used in SPLIT and JMP instructions

  Instruction(Opcode opcode_ = NIL) : opcode(opcode_) {};
};

typedef std::shared_ptr<Instruction> InstrPtr;
typedef std::vector<InstrPtr> Program;

// Convenience functions to create different instructions.
InstrPtr CreateAnyInstruction();
InstrPtr CreateCharInstruction(char c);
InstrPtr CreateMatchInstruction();
InstrPtr CreateSplitInstruction(int dst);
InstrPtr CreateJmpInstruction(int dst);

// Count the number of instructions required to represent the regexp.
int CountInstruction(std::shared_ptr<Regexp> r);

// Compile the regexp into a program.
Program CompileRegex(std::shared_ptr<Regexp> r);

void PrintInstruction(InstrPtr instr, int idx);

// Print the program (instruction addresses are replaced with ids).
void PrintProgram(Program &program);

};  // namespace azuki

#endif
