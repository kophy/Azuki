# Azuki

[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/kophy/Azuki)

Azuki is a simple regular expression engine implemented with virtual machine approach.  
Azuki是一个简单的、基于虚拟机方法实现的正则表达式引擎。  

The name `Azuki` is Japanese アズキ(read bean).  
名字`Azuki`来自日语アズキ(红豆)。

## Implementation
- [Boost Spirit](http://boost-spirit.com/home/) is used to parse regular expression into syntax tree.
- Syntax tree is compiled into program like Russ Cox's [re1](https://code.google.com/archive/p/re1/).
- NFA is simulated through “thread”s; a virtual machine runs Thompson's algorithm.
- Submatch tracking is recorded in each thread's state.

## Dependencies

To build Azuki, you need a compiler supporting C++17, CMake, Google Test and Boost installed. My environment is:

- Clang Apple LLVM version 9.0.0 (clang-900.0.39.2)
- CMake 3.10.1
- Google Test 1.8.0
- Boost 1.66.0
- Python 2.7.10

``` shell
mkdir build
cd build
cmake ..
make && make test
```

## Usage

## Quick Start
Azuki provides APIs similar to `<regex>` library. Briefly speaking, we first use `CreateMachine` to create a virtual machine from raw regular expression. Then we can detect match substring with `RegexSearch`, or replace match substring with `RegexReplace`.

```C++
// create a virtual machine from regular expression
Azuki::Machine m = Azuki::CreateMachine("^(a+)b$");
Azuki::RegexSearch(m, "aaab");  // true
Azuki::RegexSearch(m, "ac");    // false
```

## Regex Syntax

|         | Effect   | Usage   | Match | Skip |
| ------------- |:-------------:|:-----:|:-----:|:------:|
| &#124;  | alternation  | a &#124; b  | "a", "b"  | "c" |  
|  ? | match item 0 or 1 time | a?b  |  "b", "ab" | "aa", "aab" |  
|  + | match item 1 or more times | a+  | "a", "aaa" | "", "b" |  
| *  | match item 0 or more times | a&#42;b  | "b", "aab" | "ac" |  
| ^  | match start | ^a  | "a", "abc"  | "b", "ba" |  
| $  | match end  | a$  | "a", "bca"  | "b", "ab" |  
| \w | match word character | \w+ | "a", "ab" | " ", "123" |
| \d | match digit | \d+ | "123"  | " ", "ab" |
| \s | match whitespace | \s+ | " ", "\t" | "ab", "123" |
| [-] | match character in range | [a-c] | "a", "c" | "d", "1" |
| {,} | match item repeating allowed times | a{2,3} | "aa", "aaa" | "a", "ba" |

## Python Support

Azuki provides a python wrapper. 

## TODO
- [ ] non-capturing group
- [X] shorthand character classes(\\d, \\w, \\s)
- [X] escape special characters('^', '$', '(', ')', etc)
- [X] character and numerical ranges([a-c], [1-2])
- [X] curly bracket quantification ({2, 5})
- [X] regex replace

## Reference
- [Regular Expression Matching: the Virtual Machine Approach](https://swtch.com/~rsc/regexp/regexp2.html)
- [Building a Custom Expression Tree in Spirit:Qi](https://stackoverflow.com/questions/13056893/building-a-custom-expression-tree-in-spiritqi-without-utree-or-boostvariant)
