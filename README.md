# Azuki

[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/kophy/Azuki)

Azuki is a simple regular expression engine implemented with virtual machine approach.  
Azuki是一个简单的、基于虚拟机方法实现的正则表达式引擎。  

The name `Azuki` is Japanese アズキ(read bean).  
名字`Azuki`来自日语アズキ(红豆)。

## Implementation
- [Boost Spirit](http://boost-spirit.com/home/) is used to parse regular expression into syntax tree.
- Syntax tree is compiled into program like Russ Cox's [re1](https://code.google.com/archive/p/re1/).
- NFA is simulated through threads; a virtual machine runs Thompson's algorithm.
- Submatch tracking is recorded in each thread's state.

## Syntax
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

## Example
Suppose we want to match the Internet word for "laugh", 233(also 2333, 23333, ...), the regular expression can be "(233+)".

```c++
Azuki::Machine m = Azuki::CreateMachine("(233+)");
std::vector<std::string> v;
if (Azuki::RegexSearch(m, "6662333QAQ", v)) {
  std::cout << "success" << std::endl;
  for (auto &s : v)
    std::cout << s << std::endl;
} else {
  std::cout << "failure" << std::endl;
}
```
The output will be:
```
success
2333
```

The syntax tree is:
```
CAT
|--LIT 2
|--CAT
    |--LIT 3
    |--PLUS
        |--LIT 3
```

The compiled program is:
```
I0 CHAR '2'
I1 CHAR '3'
I2 CHAR '3'
I3 SPLIT I4 I5
I4 JMP I2
I5 MATCH
```

More examples can be found in `example` directory.

## TODO
- [ ] non-capturing group
- [X] shorthand character classes(\\d, \\w, \\s)
- [X] escape special characters('^', '$', '(', ')', etc)
- [X] character and numerical ranges([a-c], [1-2])
- [X] curly bracket quantification ({2, 5})
- [ ] regex replace

## Reference
- [Regular Expression Matching: the Virtual Machine Approach](https://swtch.com/~rsc/regexp/regexp2.html)
- [Building a Custom Expression Tree in Spirit:Qi](https://stackoverflow.com/questions/13056893/building-a-custom-expression-tree-in-spiritqi-without-utree-or-boostvariant)
