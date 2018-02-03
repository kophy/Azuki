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

// An instruction struct encodes information for thread to run the instruction.
// Don't create Instruction directly with constructor as it may lead to
// uninitialized fields. Use CompileRegexp below to get a whole program (a
// sequence of coherent instructions) at once.
struct Instruction {
  // Required fields.
  unsigned int idx;  // instruction index
  Opcode opcode;     // instruction opcode

  // Optional fields (depend on Opcode opcode).
  char c;                 // character to match (CHAR)
  unsigned int dst;       // destination instruction index (SPLIT and JMP)
  bool greedy;            // if true, try dst before (idx + 1) (SPLIT)
  unsigned int save_idx;  // index to save current string pointer (SAVE)
  char low_ch, high_ch;   // character lower and upper bound (RANGE)
  unsigned int
      rpctr_idx;  // index of counter of repeat times (CHECK, INCR, SET)
  int low_times, high_times;  // repeat times lower and upper bound (CHECK)
  int value;                  // value to set rpctr_idx (SET)

  bool ConsumeCharacter();
  string str();
};

typedef shared_ptr<Instruction> InstrPtr;
typedef vector<InstrPtr> Program;

// Compile into program the regular expression represented with Regexp.
// Example:
//    RegexpPtr rp = ParseRegexp("a+b");
//    Program program = CompileRegexp(rp);
Program CompileRegexp(RegexpPtr rp);

// Print the program (for debug use).
void PrintProgram(const Program &program);

};  // namespace Azuki

#endif  // __AZUKI_INSTR__
