# Day 65 — Delegation Depth and Transitive Revocation

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](delegation_tree.c)

## From Day 64
Day 64 bound a capability to one resource ID. Today a capability can be **delegated more than once**. A root grants a read-only child, which grants a read-only grandchild. Neither may add rights or switch resources.

## Model
The exercise records a handle's `depth` and limits delegation to two edges. Every handle captures a resource ID, generation and policy epoch. Checks are repeated **when used**, not only when created. A change in the resource's epoch invalidates both descendants and the original root, demonstrating **transitive revocation** without storing a pointer to every child.

Depth is a *teaching limit*, not a complete defense against arbitrary delegation. The underlying resource stays allocated to avoid use-after-free.

## Run
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 delegation_tree.c -o delegation_tree
./delegation_tree
gdb -q ./delegation_tree
```
In GDB or Ghidra, trace the tests of `depth`, `rights`, `generation` and `epoch` before the resource is accessed.

## Exercises
1. Why do descendants remain read-only?
2. How does one epoch change invalidate a whole delegation family?
3. What is the difference between a **resource-wide** revocation and revoking just one child?
4. Why doesn't this model handle freed resources, concurrent threads or epoch overflow?
5. What would a real capability system need to revoke only one branch?

## Limits
Offline, single-threaded C fixture. No OS privilege boundary or production security guarantee.
