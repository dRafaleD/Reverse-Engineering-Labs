# Day 58 — Pointer Aliasing and In-Place Mutation Tracing

[English](notes.md) · [Türkçe](notes.tr.md) · [C exercise](aliasing.c)

## Goal
After Day 57's state mutations, explore **pointer aliasing**: two parameters can hold the address of the *same* object. In `add_events(a,b,count)`, distinct pointers update two counters, but `a == b` means there is only one counter to update. A decompiler's distinct variable names do not prove distinct storage.

## Safety invariant
Validate the pointers, positive amount and unsigned overflow **before** changing anything. Rejected calls preserve both counters. The exercise checks non-aliasing, aliasing, overflow, zero increment and null-pointer cases.

## Reproduce
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 aliasing.c -o aliasing
./aliasing
gdb -q ./aliasing
```
In GDB: `break add_events`, `run`, `print a`, `print b`, `print a == b`. Continue to the aliased call. Inspect memory stores in Ghidra and compare a `-O2 -g` build.

## Questions
Why can two function arguments share an address? What prevents the second increment? Why check overflow before the first write? What would change under concurrency?

## Limits
Harmless local C fixture; neither atomicity nor thread safety is guaranteed.
