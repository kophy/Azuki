#ifndef __AZUKI_INSTR__
#define __AZUKI_INSTR__

#include "common.h"
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

// The instruction struct encodes information to run an instruction.
struct Instruction {
  // Required fields.
  unsigned int idx;  // instruction index
  Opcode opcode;     // instruction opcode

  // Optional fields.
  char c;             // character to match (CHAR)
  unsigned int dst;   // destination instruction index (SPLIT and JMP)
  bool greedy;        // if true, try dst before (idx + 1) (SPLIT)
  unsigned int slot;  // slot to save string pointer (SAVE)

  Instruction(Opcode opcode) : opcode(opcode) {}
  string str();
};

typedef shared_ptr<Instruction> InstrPtr;
typedef vector<InstrPtr> Program;

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

// Compile into program the regular expression represented with Regexp.
Program CompileRegexp(RegexpPtr rp);

// Print the program (for debug use).
void PrintProgram(const Program &program);

};  // namespace Azuki

#endif  // __AZUKI_INSTR__
