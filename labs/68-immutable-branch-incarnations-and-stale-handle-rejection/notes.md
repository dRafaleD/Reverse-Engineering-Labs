# Day 68 — Immutable Branch Incarnations and Stale Handle Rejection

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](branch_incarnation.c)

Day 67 checked ancestor epochs. This exercise isolates a subtle lifecycle invariant: reissuing a branch must not reactivate a reader that was previously revoked. Instead of modifying an old branch's parent binding in place, the fixture leaves the old branch inactive and creates a distinct branch incarnation. Each reader checks its captured identity and active state before reading. The eight assertions cover initial access, revocation, reissue, stale identity and null output.

**Analysis exercise:** locate the active-state and identity comparisons in Ghidra or break at read_value in GDB. Compare an old reader with a newly issued reader. Why is a copied numeric identity insufficient as a real security boundary? Why must pointers remain valid?

This also illustrates why mutable ancestor bindings in the previous toy lab should not be interpreted as a guarantee against stale-handle resurrection. Real implementations need immutable issuance records, non-reused identities, safe lifetime management and synchronization. All objects here remain allocated; no arbitrary memory access or concurrency is modeled.

## Run
```bash
cd labs/68-immutable-branch-incarnations-and-stale-handle-rejection
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 branch_incarnation.c -o branch_incarnation
./branch_incarnation
```
