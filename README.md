# Reverse-Engineering-Labs

[🇬🇧 English](README.md) | [🇹🇷 Türkçe](README.tr.md)

A reverse engineering course starting from the basics.

This repository documents my journey of learning reverse engineering from the fundamentals.

The goal is to understand how compiled programs work internally by exploring topics such as C, assembly, memory, binary formats, debugging, and static/dynamic analysis.

## Labs

- [Day 1 — C Fundamentals](labs/01-c-fundamentals/notes.md)
- [Day 2 — First Ghidra Analysis](labs/02-first-ghidra-analysis/notes.md)
- [Day 3 — C Variables Inside a Binary](labs/03-c-variables-in-binary/notes.md)
- [Day 4 — C Functions and Calling Conventions](labs/04-functions-and-calling-convention/notes.md)
- [Day 5 — Stack Frames and Local Variables](labs/05-stack-frames-and-locals/notes.md)
- [Day 6 — Conditions and Conditional Jumps](labs/06-conditions-and-conditional-jumps/notes.md)
- [Day 7 — Loops and Backward Jumps](labs/07-loops-and-backward-jumps/notes.md)
- [Day 8 — Arrays and Pointer Basics](labs/08-arrays-and-pointers/notes.md)
- [Day 9 — Structs and Memory Layout](labs/09-structs-and-memory-layout/notes.md)
- [Day 10 — Switch Statements and Jump Tables](labs/10-switch-and-jump-tables/notes.md)
- [Day 11 — Strings and Character Arrays](labs/11-strings-and-char-arrays/notes.md)
- [Day 12 — malloc and Heap Basics](labs/12-malloc-and-heap-basics/notes.md)
- [Day 13 — Function Pointers and Indirect Calls](labs/13-function-pointers-and-indirect-calls/notes.md)
- [Day 14 — Global and Static Variables](labs/14-global-and-static-variables/notes.md)
- [Day 15 — Signed and Unsigned Integers](labs/15-signed-and-unsigned-integers/notes.md)
- [Day 16 — Recursion and Nested Stack Frames](labs/16-recursion-and-nested-stack-frames/notes.md)
- [Day 17 — Bitwise Operations and Flags](labs/17-bitwise-operations-and-flags/notes.md)
- [Day 18 — Library Calls, PLT/GOT and String Comparison](labs/18-library-calls-plt-got/notes.md)
- [Day 19 — Function Pointers Revisited: Callbacks and Indirect Control Flow](labs/19-callbacks-and-indirect-control-flow/notes.md)
- [Day 20 — ELF Sections, Symbols and Stripped Binaries](labs/20-elf-sections-symbols-and-stripping/notes.md)
- [Day 21 — GDB Basics: Breakpoints, Registers and Stepping Through Execution](labs/21-gdb-breakpoints-registers-and-stepping/notes.md)
- [Day 22 — Process Memory Layout, Virtual Memory, ASLR and /proc Maps](labs/22-process-memory-layout-aslr-and-proc-maps/notes.md)
- [Day 23 — Validation Logic, Data Flow and Branch Tracing in Ghidra + GDB](labs/23-validation-data-flow-and-branch-tracing/notes.md)
- [Day 24 — System Calls, libc Wrappers, strace and ltrace](labs/24-syscalls-libc-strace-and-ltrace/notes.md)

## Learning Approach

Each lab starts with a small C example, then connects the source code to the compiled binary through tools such as Ghidra. The focus is on recognizing patterns gradually instead of memorizing assembly instructions all at once.

## What is Reverse Engineering?

Reverse engineering is the process of analyzing a compiled program to understand its structure, behavior, and internal logic without access to the original source code.

In this repository, I will explore concepts such as:

- C and compiled programs
- x86/x86-64 assembly
- Registers and CPU instructions
- Stack and heap memory
- Function calls and calling conventions
- ELF and PE binaries
- Static analysis
- Dynamic analysis
- Debugging
- Ghidra and GDB

It serves as both a quick refresher for myself and a guide for others interested in this subject.
