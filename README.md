Good Job. Great Effort. Ride on to MP4 and keep going. Very soon we will hit the jackpot. I will see you on Tuesday. Have a great weekend. 



This project is for COSC375 Compiler Construction.

The goal is to write a compiler to translate COOL to LLVM assembly code.


mp1: lexer and parser
=========================================================================

test file: src/bison_test_good.cl, src/bison_test_bad.cl

testing:

compile lexer: make lexer

run lexer: ./lexer filename.cl

compile parser: make parser

run parser: ./lexer filename.cl |./parser


Hello< Yijun. Thank you for the good work you are doing. Keep up with the effort and be sure to appreciate how much you would learned by the end of the course. Relax for a bit and concentrate on your homeworks while l post MP3.
Good Job.


mp2: code generation 1 (cgen-1)
=========================================================================

test file: test-1

testing:

make filename.ll

make filename.bc

make filename.s


mp3: code generation
=========================================================================

mp3: code generation

original zip file:

(1) file reference-binaries

(2) test-2

(3) cgen-case.cc

(4) cool-tree.handcode.h: copied to src


copy from mp2:

(1) file cool-support

(2) file src: replace cool-tree.handcode.h by the new version from mp3.tar1.gz

(3) Makefile.common

(4) README.md


test:

make filename.ast

make filename.ll

make filename.bc

make filename.exe

make filename.out

make filename.verify

make file-opt.bc


mp4: register allocator
=========================================================================

For step 1.5 

llc --regalloc=simple filename.ll



testing

llc --verify-machineinstrs filename.ll

llc --regalloc=simple filename.ll

llc -O0 filename.ll 

llc -O1 filename.ll

llc -O2 filename.ll
