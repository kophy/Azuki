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
  NIL,  // suggest the instruction is invalid
  SPLIT,
  JMP
};

struct Instruction {
  unsigned int idx;  // instruction index
  Opcode opcode;     // instruction type
  char c;            // character to match (CHAR)
  unsigned int dst;  // destination instruction index (SPLIT and JMP)

  Instruction(Opcode opcode = NIL) : opcode(opcode) {}
  std::string to_string();
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
int CountInstruction(RegexpPtr r);

// Compile the regexp into a program.
Program CompileRegex(RegexpPtr r);

// Print the program (instruction addresses are replaced with ids).
void PrintProgram(Program &program);

};  // namespace azuki

#endif
