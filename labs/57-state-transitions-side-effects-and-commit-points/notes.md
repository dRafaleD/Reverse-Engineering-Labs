# Day 57 — State Transitions, Side Effects and Commit Points

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](transition_side_effects.c)

## Why this follows Day 56
Day 56 studied errors, retries and recovery. This lab looks beyond a state label: a compiled program can also update **side effects**, such as completion and retry counters. A rejected event should not silently mutate those fields. When reverse engineering, we need to trace both **control flow** (which branch ran?) and **data flow** (which memory locations changed?).

## Small, explicit model
The C fixture uses states INIT, ACTIVE, FAILED and DONE, and events START, COMPLETE, FAIL, RETRY and RESET.

| Current state | Event | Next state | Other effect |
| --- | --- | --- | --- |
| INIT | START | ACTIVE | none |
| ACTIVE | COMPLETE | DONE | completed + 1 |
| ACTIVE | FAIL | FAILED | none |
| FAILED | RETRY | ACTIVE | retries + 1 |
| ACTIVE / FAILED / DONE | RESET | INIT | none |

All other state/event pairs are **rejected**. Their state and both counters must stay unchanged. RESET does not clear the counters: they represent cumulative metrics in this teaching model, not per-session counters.

A **commit point** here means the code location where a permitted branch updates a field. It is a logical point of mutation, **not** a transactional, crash-safe or thread-safe commit. The two writes in ACTIVE/COMPLETE are not atomic as a group.

## Evidence in the fixture
The test uses four states × five events = **20 combinations** and independent expected state, acceptance and counter-delta tables. It starts each pair with nonzero counters to catch accidental changes.

It also replays a nine-event trace:
1. START → FAIL → RETRY → COMPLETE: one completion and one retry.
2. A duplicate COMPLETE in DONE is rejected: the completion counter stays at one.
3. RESET returns to INIT, preserving cumulative counters.
4. RETRY in INIT is rejected.
5. START → COMPLETE brings the completion counter to two.

The assertions check the **specified local model**, not the security of arbitrary binaries.

## Build and inspect on Linux
~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 transition_side_effects.c -o transition_side_effects
./transition_side_effects
gdb -q ./transition_side_effects
objdump -d -M intel transition_side_effects | less
~~~

Inside GDB:
~~~text
(gdb) break apply
(gdb) run
(gdb) print *w
(gdb) disassemble apply
(gdb) next
(gdb) print *w
~~~

Repeat or continue to compare accepted and rejected calls. In Ghidra, find the apply function and locate the memory writes corresponding to Workflow.state, Workflow.completed and Workflow.retries. Compare -O0 with -O2 -g: branch shapes and instruction order may change, so reason from observable behavior, not just the decompiler's C-like display. Test runs were verified locally with GCC at both optimization levels.

## Exercises
1. Draw the full 4×5 table, marking accepted and rejected cells.
2. Which write increments completed? What prevents duplicate COMPLETE from incrementing it again?
3. Move the counter increment before its guard deliberately. Which test catches the unintended side effect?
4. Set a breakpoint before and after FAILED/RETRY; inspect the Workflow struct in memory.
5. Why are these single-threaded assertions not proof of atomicity, persistence or authorization?

## Scope and limits
This is a local, harmless C reverse-engineering fixture. It uses neither user accounts nor external targets. Real services also need synchronization, crash recovery, durable storage, overflow handling and authorization checks. Do not infer security guarantees from a passing finite test table.
