#ifndef __AZUKI_INSTR__
#define __AZUKI_INSTR__

#include <memory>
#include <string>
#include <vector>
#include "regexp.h"

namespace azuki {

// Instruction opcodes.
enum Opcode { ANY, CHAR, MATCH, SAVE, SPLIT, JMP };

// An instruction struct encodes information to run an instruction.
struct Instruction {
  unsigned int idx;  // instruction index
  Opcode opcode;     // instruction type
  char c;            // character to match (only used in CHAR)
  unsigned int dst;  // destination instruction index (used in SPLIT and JMP)
  unsigned int slot; // slot to save string pointer

  Instruction(Opcode opcode) : opcode(opcode) {}
  std::string str();
};

typedef std::shared_ptr<Instruction> InstrPtr;

// Convenience functions to create different instructions.
InstrPtr CreateAnyInstruction();
InstrPtr CreateCharInstruction(char c);
InstrPtr CreateMatchInstruction();
InstrPtr CreateSaveInstruction(int slot);
InstrPtr CreateSplitInstruction(int dst);
InstrPtr CreateJmpInstruction(int dst);

typedef std::vector<InstrPtr> Program;

// Compile the regexp into a program.
Program CompileRegex(RegexpPtr r);

// Print the program (for debug use).
void PrintProgram(const Program &program);

};  // namespace azuki

#endif
