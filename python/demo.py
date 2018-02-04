#!/usr/bin/env python

import pyazuki
import sys

def vec2list(vec):
    l = []
    for i in range(vec.size()):
        l.append(vec[i])
    return l

def demo():
    print "======================================"
    rp = pyazuki.ParseRegexp("(a+)b")
    print "The regexp is:\n"
    pyazuki.PrintRegexp(rp)

    print "======================================"
    program = pyazuki.CompileRegexp(rp)
    print "The program is:\n"
    pyazuki.PrintProgram(program)

    machine = pyazuki.CreateMachine("(a+)b")
    s = "aabcdabe"

    print "======================================"
    print "An example of regex search."
    print "Find \"(a+)b\" in \"%s\".\n" %(s)
    result = pyazuki.MatchResult()
    while pyazuki.RegexSearch(machine, s, result, True):
        print "begin = %d, end = %d, capture = " %(result.begin, result.end),
        print vec2list(result.capture)

    print "======================================"
    print "An example of regex replace."
    print "Replace \"(a+)b\" with \"$0f\" in \"%s\".\n" %(s)
    print pyazuki.RegexReplace(machine, s, "$0f", True)


if __name__ == "__main__":
    demo()
