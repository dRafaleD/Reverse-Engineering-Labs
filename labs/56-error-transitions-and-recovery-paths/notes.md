# Day 56 — Error Transitions and Recovery Paths in Compiled Programs

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](recovery.c)

## From coverage to failure handling
Day 55 checked all nine state/event combinations of a simple workflow. Real software also needs to handle failures, retries, and resets. A **recovery edge** deserves the same scrutiny as a successful edge: does the program leave an error state only after an explicitly allowed event?

This safe C fixture has three states: INIT, ACTIVE and FAILED. Events are START, FAIL, RETRY and RESET. Allowed transitions are INIT/START → ACTIVE, ACTIVE/FAIL → FAILED, FAILED/RETRY → ACTIVE, and ACTIVE or FAILED / RESET → INIT. Other combinations are rejected **without mutating state**. In particular, INIT/RETRY must not grant ACTIVE.

## Reverse-engineering questions
- **State:** the current condition of the workflow; not a user's authenticated identity.
- **Transition guard:** the condition permitting a move.
- **Rejected event:** an event that leaves the state unchanged; the return value also records rejection.
- **Invariant:** a property that must hold on every checked call. Our fixture verifies rejection leaves the state unchanged and every output is a defined state.
- **Recovery path:** a specifically permitted route out of FAILED; it must not accidentally become a shortcut from INIT.

The test program exhausts the 3×4 combinations using an independent expected table, then simulates a failure and retry trace. Exhaustive *finite-table* coverage is not exhaustive security testing.

## Local practice
Use GCC/GDB on your own Linux machine:

~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 recovery.c -o recovery
./recovery
gdb -q -batch -ex 'break step' -ex run -ex 'info args' -ex 'bt' -ex quit ./recovery
objdump -d -M intel recovery | less
~~~

Find the comparison branches in Ghidra and compare with a second build using -O2 -g. Optimization can change the assembly layout while preserving the modeled behavior.

## Exercises
1. Draw the three nodes and five allowed edges; mark every rejected pair.
2. Why would accepting INIT/RETRY be an invalid recovery path?
3. Temporarily change the FAILED/RETRY behavior and explain which assertion catches it.
4. Compare an observed GDB call with the independently specified expected table.
5. What additional safeguards would a real stateful service need around concurrent requests, persistent state and authorization?

## Limits
The fixture is a deterministic teaching model. A checked state transition is **not** a substitute for real authentication, authorization, atomic storage or input validation. No external binaries or unauthorized systems are involved.
