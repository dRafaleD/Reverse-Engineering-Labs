# Day 53 — String Comparisons and Runtime Branch Evidence

[English](notes.md) · [Türkçe](notes.tr.md) · [Practice](branches.c)

## Learning goals
Day 52 located string references in a binary. Now connect those **static cross-references** to **observed execution** using GDB. Explain exactly what a breakpoint proves and what it does not.

## Concept 1: static vs. dynamic evidence
A cross-reference in Ghidra says a piece of code *may refer* to a constant. It does not say that instruction was executed. A runtime breakpoint and recorded arguments support the narrower conclusion that this function was reached with particular values **during this run**. They do not establish behavior on every possible input.

## Concept 2: branching and comparisons
The sample uses `strcmp`: zero means the strings compare equal, not that the return value is "false" in a security sense. The order of `if` checks matters. Inspect `classify` and the three calls in `main`. At `-O0`, GDB should expose arguments relatively clearly; optimized binaries may rearrange or inline code.

## Lab — Linux, GCC and GDB
```bash
gcc -std=c11 -Wall -Wextra -g -O0 -fno-omit-frame-pointer branches.c -o branches
./branches
gdb -q -batch -ex 'break classify' -ex run -ex 'info args' -ex 'bt' -ex continue -ex quit ./branches
```
Expected program labels are `known`, `review`, and `other`, one for each fabricated input. Compare Ghidra's XREF list for `SAFE-LAB` with the call observed in GDB.

## Tasks
1. Record the first `classify` argument at the breakpoint. Which call supplied it?
2. Change the second input to `SAFE-LAB`, rebuild, and predict output.
3. Compile again with `-O2 -g`; explain any differences in debugging visibility.
4. Distinguish code presence, branch execution, and actual malicious intent.
5. Note compiler version, CPU architecture and exact commands for reproducibility.

## Limitations
This is harmless local C code. No third-party binary is executed, and program output is not proof of maliciousness. Compiler output depends on toolchain and ABI.
