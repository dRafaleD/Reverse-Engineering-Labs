# Day 59 — Pointer Chains, Field Offsets and Alias Tracing

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](pointer_chain.c)

## Connection to Day 58
Day 58 showed that two pointer parameters can alias the same object. A compiled program may also follow a **pointer chain**: `Handle -> View -> Record -> count`. Seeing two different chains does not establish two different underlying records.

## Concepts
- **Pointer dereference:** following a stored address to access an object.
- **Field offset:** a struct member's byte displacement from the start of its structure. `offsetof` reports it for **this build**; padding and ABI can affect exact values.
- **Aliasing:** `ha` and `hb` have different intermediate Views but initially reach the same Record.
- **Repointing:** changing `b.target` makes only `hb` reach a second Record.
- **Validate before mutate:** null links, zero increments and unsigned overflow are rejected before any write.

## Reproduce and investigate
~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 pointer_chain.c -o pointer_chain
./pointer_chain
gdb -q ./pointer_chain
~~~
In GDB: `break add_via_chain`, `run`, `print handle`, `print *handle` and `print *handle->view->target`. Continue until the third accepted call and compare targets. In Ghidra, reconstruct the pointer loads and final field write; compare an `-O2 -g` build.

## Practice questions
1. Why do different intermediate addresses not prove different final targets?
2. How can a series of loads reveal nested pointer access in disassembly?
3. Why should field offsets not be hardcoded across all architectures?
4. Which guard prevents unsigned wraparound before mutation?
5. Would this fixture be safe with concurrent writers?

## Limits
Local C teaching fixture only: no outside binaries, no concurrency guarantees and no claim that observed assembly reconstructs original source exactly.
