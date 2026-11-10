# Day 55 — State Machine Invariants and Transition Coverage

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](transition_tests.c)

## Goal
Day 54 reconstructed a three-state workflow from branches. This day asks a different question: **did we test every state/event combination, including rejected transitions?** A working demonstration of the happy path is not a complete behavioral specification.

## The transition table
Our synthetic machine has states `IDLE`, `READY`, `DONE` and events `START`, `CONFIRM`, `RESET`. The allowed transitions are:
- `IDLE + START → READY`
- `READY + CONFIRM → DONE`
- `DONE + RESET → IDLE`

For the six remaining combinations, the invariant is **reject and retain current state**. For *every* pair, the returned state must belong to the defined state set.

The C fixture iterates through all **3 × 3 = 9** combinations, derives expected results from an independently written table, and compares against `transition()`. It checks that the implementation and the table agree, not that an unknown binary is secure.

## Reproduce on Linux
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 transition_tests.c -o transition_tests
./transition_tests
gdb -q -batch -ex 'break transition' -ex run -ex 'info args' -ex 'bt' -ex quit ./transition_tests
objdump -d -M intel transition_tests | less
```
Expected output: nine PASS lines, followed by `9/9 transitions verified`. At `-O0`, the GDB breakpoint usually exposes C argument names. Rebuild with `-O2 -g` and compare the compiler's branch structure.

## Investigation tasks
1. Draw the 3×3 state/event matrix. Which cells are allowed and which denied?
2. Why can the three successful transitions alone leave a dangerous bug hidden?
3. Change `transition()` so `READY + RESET` is mistakenly accepted. Which assertion fails?
4. Compare how `switch`, `if`, and optimized code appear in Ghidra. Is assembly layout a proof of semantics?
5. How would you extend the model for an explicit ERROR state without silently allowing transitions from it?
6. Explain the gap between **all defined pairs tested** and **all possible runtime scenarios tested** (concurrency, memory corruption, external inputs).

## Evidence and limits
Local, harmless C and no external binary. Assertions cover the specified finite model only. Passing tests does not prove that the compiled binary has no vulnerabilities or that every state transition in a real-world workflow is safe.
