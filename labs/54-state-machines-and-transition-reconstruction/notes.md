# Day 54 — Reconstructing State Machines from Branches

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](states.c)

## Goal
Day 53 connected string comparisons to observed branches. This time we reconstruct a **finite-state machine** from a small compiled C program, distinguishing a code path that exists from a transition that actually happened.

## Model before disassembly
A state machine has a finite set of **states**, **events**, and a transition function. Our harmless workflow has `IDLE`, `READY`, and `DONE`. Valid moves are `IDLE + START → READY`, `READY + CONFIRM → DONE`, and `DONE + RESET → IDLE`. Every other state/event pair is rejected **without changing state**.

The C program returns a `Result` containing both the next state and an `accepted` flag. This distinction matters: an unchanged state could be a valid no-op in some designs, so state alone is not necessarily enough to infer rejection.

## Reproducible lab (Linux + GCC + GDB)
From this folder:
```bash
gcc -std=c11 -Wall -Wextra -g -O0 -fno-omit-frame-pointer states.c -o states
./states
gdb -q -batch -ex 'break transition' -ex run -ex 'info args' -ex 'bt' -ex continue -ex quit ./states
objdump -d -M intel states | less
```
Expected demo: the premature `CONFIRM` is rejected, then `START`, `CONFIRM`, `RESET` are accepted. The executable uses assertions to check the transition contract.

## Investigation
1. Draw a diagram with three nodes and three allowed directed edges. Mark rejected cases separately.
2. Locate `transition` in Ghidra. Identify comparisons, conditional branches, and the return value layout. A compiler might use a jump table, but is not obliged to.
3. At the first breakpoint, record `current` and `event`. Why does one breakpoint not prove all possible transitions?
4. Compile with `-O2 -g` into a different binary; compare control flow and debugger visibility.
5. Add another **invalid** state/event pair and an assertion. Explain what direct observation proves and what remains a hypothesis.

## Security relevance and limits
Authorization workflows and network protocols often depend on valid state transitions. This fixture teaches reverse-engineering **technique**, not a real authorization system. Enumeration names and debug symbols may be absent in stripped binaries. Use only locally compiled or authorized programs.
