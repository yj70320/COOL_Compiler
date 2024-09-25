This project is for COSC375 Compiler Construction, by Yijun Wu

The goal is to write a compiler to translate COOL to MIPS.


MP1: lexer and parser

compile lexer: make lexer
run lexer: ./lexer filename.cl

compile parser: make parser
run parser: ./lexer filename.cl |./parser

When run parser, if the program has more than 20 errors, it will list first 20 errors, and stop the program. If errors number is under 20, it will show all the error.

If there are any errors in the program, after the error messages, the parser will show "Compilation halted due to lex and parse errors." and stop the program. This is from /cool-support/src/parser-phase.cc file.

test files:
flex_test.cl
bison_test_good.cl
bison_test_bad.cl
