# Azuki

[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/kophy/Azuki)

Azuki is a simple regular expression engine implemented with virtual machine approach.  
Azuki是一个简单的、基于虚拟机方法实现的正则表达式引擎。  

The name `Azuki` is Japanese アズキ(read bean).  
名字`Azuki`来自日语アズキ(红豆)。

## Introduction

Azuki is basically a C++ version of Russ Cox's [re1](https://code.google.com/archive/p/re1/), but supports more regex syntax and operations.

- [Boost Spirit](http://boost-spirit.com/home/) is used to parse regular expression into syntax tree.
- Syntax tree is compiled into program like Russ Cox's [re1](https://code.google.com/archive/p/re1/).
- Nondeterministic finite automaton is simulated through “thread”s; a virtual machine runs Thompson's algorithm.
- Submatch tracking is recorded in each thread's state.

The corresponding program for regular expression "a+b" is:
```
I0 CHAR 'a'
I1 SPLIT I2 I3
I2 JMP I0
I3 CHAR 'b'
I4 MATCH
```

## Dependencies

To build Azuki, you need a compiler supporting C++17, CMake, Google Test and Boost installed.  
My environment is:

- Clang Apple LLVM version 9.0.0 (clang-900.0.39.2)
- CMake 3.10.1
- Google Test 1.8.0
- Boost 1.66.0
- Python 2.7.10

Once installed these dependencies, build Azuki and run tests with commands:
``` shell
mkdir build
cd build
cmake ..
make && make test
```

## Usage

### Quick Start
Briefly speaking, we use `CreateMachine` to create a virtual machine from raw regular expression, then we can detect match substrings with `RegexSearch`, or replace them with `RegexReplace`.

#### Example 1
Detect whether input string contains pattern "a+b".

```C++
Azuki::Machine m = Azuki::CreateMachine("a+b");
Azuki::RegexSearch(m, "aab");   // true
Azuki::RegexSearch(m, "ac");    // false
```

#### Example 2
Run `RegexSearch` through input string to find all substrings match pattern "(a+)(b)", and print capturing groups in each substring:
```C++
Azuki::Machine m = Azuki::CreateMachine("(a+)(b)");
Azuki::MatchResult result;
while (Azuki::RegexSearch(m, "aabcdabe", result)) {
  for (auto &sub : result.capture)
    std::cout << sub << ' ';
  std::cout << std::endl;
}
```
The output will be:
```
aa b
a b
```

#### Example 3
Replace substring matches with "(a+)b" in input string with new substring specified by format string "$0c". "$0" is the 0th capturing group, which is the "a+" in "(a+)b".

```C++
Azuki::Machine m = Azuki::CreateMachine("(a+)b");
Azuki::RegexReplace(m, "daabe", "$0c");   // "daace"
```
---

Check file `src/azuki.h` for detailed guide.

### Regex Syntax

|         | Effect   | Usage   | Match | Skip |
| ------------- |:-------------:|:-----:|:-----:|:------:|
| &#124;  | alternation  | a &#124; b  | "a", "b"  | "c" |  
|  ? | match item 0 or 1 time | a?b  |  "b", "ab" | "aa", "aab" |  
|  + | match item 1 or more times | a+  | "a", "aaa" | "", "b" |  
| *  | match item 0 or more times | a&#42;b  | "b", "aab" | "ac" |  
| ^  | match begin | ^a  | "a", "abc"  | "b", "ba" |  
| $  | match end  | a$  | "a", "bca"  | "b", "ab" |  
| \w | match any word character | \w+ | "a", "ab" | " ", "123" |
| \d | match any digit | \d+ | "123"  | " ", "ab" |
| \s | match any whitespace | \s+ | " ", "\t" | "ab", "123" |
| [c1-c2] | match character in range [c1, c2] | [a-c] | "a", "c" | "d", "1" |
| {t1,t2} | match item repeating allowed times [t1, t2] | a{2,3} | "aa", "aaa" | "a", "ba" |

*The `{}` operator also supports two variants. `{t}` matches item repeating exactly `t` tiems, `{t,}` matches item repeating at least `t` times.*

## Python Support

Azuki provides a python wrapper through Boost.Python.  
After running `make`, copy `pyazuki.so` under `build/python` directory to whatever directory you like, in that directory you can use Python to play with Azuki:
```
import pyazuki
print pyazuki.RegexReplace(machine, s, "$0c", True)
```
Check file `python/demo.py` for more examples.

## Reference
- [Regular Expression Matching: the Virtual Machine Approach](https://swtch.com/~rsc/regexp/regexp2.html)
- [C++ standard regular expressions library](http://en.cppreference.com/w/cpp/regex)
- [Building a Custom Expression Tree in Spirit:Qi](https://stackoverflow.com/questions/13056893/building-a-custom-expression-tree-in-spiritqi-without-utree-or-boostvariant)

## TODO
- [ ] non-capturing group
- [X] shorthand character classes(\\d, \\w, \\s)
- [X] escape special characters('^', '$', '(', ')', etc)
- [X] character and numerical ranges([a-c], [1-2])
- [X] curly bracket quantification ({2, 5})
- [X] regex replace
