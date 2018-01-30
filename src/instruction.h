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
  CHECK,
  INCR,
  MATCH,
  RANGE,
  SAVE,
  SET,
  SPLIT,
  JMP
};

// The instruction struct encodes information to run an instruction.
struct Instruction {
  // Required fields.
  unsigned int idx;  // instruction index
  Opcode opcode;     // instruction opcode

  // Optional fields.
  char c;                // character to match (CHAR)
  unsigned int dst;      // destination instruction index (SPLIT and JMP)
  bool greedy;           // if true, try dst before (idx + 1) (SPLIT)
  unsigned int slot;     // index of slot to save string pointer (SAVE)
  char low_ch, high_ch;  // character lower and upper bound (RANGE)
  unsigned int counter;  // index of counter for repeat times (CHECK, INCR, SET)
  int low_times, high_times;  // repeat times lower and upper bound (CHECK)
  int value;                  // value to set counter (SET)

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
InstrPtr CreateCheckInstruction(unsigned int counter, int low_times,
                                int high_times);
InstrPtr CreateIncrInstruction(unsigned int counter);
InstrPtr CreateMatchInstruction();
InstrPtr CreateRangeInstruction(char low_ch, char high_ch);
InstrPtr CreateSaveInstruction(unsigned int slot);
InstrPtr CreateSetInstruction(unsigned int counter, int value);
InstrPtr CreateSplitInstruction(int dst, bool greedy = false);
InstrPtr CreateJmpInstruction(int dst);

// Compile into program the regular expression represented with Regexp.
Program CompileRegexp(RegexpPtr rp);

// Print the program (for debug use).
void PrintProgram(const Program &program);

};  // namespace Azuki

#endif  // __AZUKI_INSTR__
