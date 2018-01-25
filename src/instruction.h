#ifndef __AZUKI_INSTR__
#define __AZUKI_INSTR__

#include <memory>
#include <string>
#include <vector>
#include "regexp.h"

namespace Azuki {

// Instruction opcodes.
enum Opcode {
  ANY,
  ANY_WORD,
  ANY_DIGIT,
  ANY_SPACE,
  CHAR,
  MATCH,
  SAVE,
  SPLIT,
  JMP
};

// An instruction struct encodes information to run an instruction.
struct Instruction {
  unsigned int idx;   // instruction index
  Opcode opcode;      // instruction type
  char c;             // character to match (only used in CHAR)
  unsigned int dst;   // destination instruction index (used in SPLIT and JMP)
  bool greedy;        // if true, try dst before (pc + 1) (only used in SPLIT)
  unsigned int slot;  // slot to save string pointer (only used in SAVE)

  Instruction(Opcode opcode) : opcode(opcode) {}
  std::string str();
};

typedef std::shared_ptr<Instruction> InstrPtr;
typedef std::vector<InstrPtr> Program;

// Convenience functions to create different instructions.
InstrPtr CreateAnyInstruction();
InstrPtr CreateAnyWordInstruction();
InstrPtr CreateAnyDigitInstruction();
InstrPtr CreateAnySpaceInstruction();
InstrPtr CreateCharInstruction(char c);
InstrPtr CreateMatchInstruction();
InstrPtr CreateSaveInstruction(int slot);
InstrPtr CreateSplitInstruction(int dst, bool greedy = false);
InstrPtr CreateJmpInstruction(int dst);

// Compile the regexp into a program.
Program CompileRegexp(RegexpPtr rp);

// Print the program (for debug use).
void PrintProgram(const Program &program);

};  // namespace Azuki

#endif
