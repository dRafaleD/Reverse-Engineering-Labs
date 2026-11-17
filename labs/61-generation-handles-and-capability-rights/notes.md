# Day 61 — Generation-Checked Handles and Capability Rights

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](capability_handles.c)

## From Day 60
Day 60 used a **generation counter** to reject stale handles after a logical slot was recycled. Day 61 adds a second, independent question: **even if this handle still identifies the current object, is it allowed to read or modify it?**

## Concepts
- **Identity validity:** a non-null, active slot with the same generation as the handle.
- **Capability:** a handle's scoped permissions; here bit flags `CAN_READ` and `CAN_WRITE`.
- **Least privilege:** a read-only handle cannot modify a value, and a write-only handle cannot read it.
- **Stale capability:** after the slot is recycled, both old handles fail, regardless of their permissions.

The validator checks object liveness, generation and required rights before any read or write. A recycled slot stays allocated to **avoid use-after-free** in this educational fixture.

## Run and inspect
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 capability_handles.c -o capability_handles
./capability_handles
gdb -q ./capability_handles
```
In GDB, break on `valid` and compare `h->generation`, `h->slot->generation` and `h->rights`. In Ghidra, identify the bit-mask check and the branch that rejects the old generation. Compare an `-O2` build.

## Exercises
1. Why do generation validation and permission validation answer different questions?
2. What happens if the same slot is reused but the old handle retains `CAN_WRITE`?
3. Why is a capability bitmask **not** a replacement for operating-system authorization?
4. What would be needed if the underlying `Slot` were freed?
5. What synchronization would be required with concurrent readers/writers?

## Boundaries
No general allocator, security boundary, atomicity or thread-safety guarantee. It is a deterministic C reverse-engineering exercise.
