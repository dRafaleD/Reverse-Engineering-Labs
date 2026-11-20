# Day 64 — Capability Resource Scopes and Delegation

[English](notes.md) · [Türkçe](notes.tr.md) · [C exercise](scoped_handles.c)

## From Day 63
Attenuation stops a read-only handle from acquiring write permission. **Resource scope** imposes another boundary: even a valid handle must not be silently redirected to a different resource.

## Demonstration
Each in-memory `Resource` has an immutable-for-this-exercise ID, generation, policy epoch, active state and allowed rights. A `Handle` carries the ID it was granted for. Before reading, writing or delegating, the fixture verifies **matching resource ID, generation, epoch, and permissions**. Delegation may reduce rights but cannot change the target ID. A forged ID and stale epoch are rejected.

## Run
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 scoped_handles.c -o scoped_handles
./scoped_handles
gdb -q ./scoped_handles
```
Break in `delegate` and `valid` to trace the identity and rights checks; locate them in optimized disassembly with Ghidra.

## Questions
1. Why is a resource identifier an authorization dimension distinct from rights?
2. Why must a delegated child preserve its parent's scope?
3. What happens to delegated references after an epoch change?
4. Is a copied ID cryptographically unforgeable? What would a real capability need?
5. Why does this single-threaded model not solve use-after-free or race conditions?

## Scope
A permanently allocated, safe C teaching object—not an OS capability system, secure handle allocator or thread-safe policy.
