# Day 60 — Pointer Lifetime and Generation-Checked Handles

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](generation_handles.c)

## Continuation of Day 59
Day 59 reconstructed pointer chains and field offsets. Now consider what happens when a pointer still refers to a **logical slot** whose contents have changed. Address equality does **not** prove object-identity continuity: a memory slot may be reused.

## Terms
- **Lifetime:** the period an object can validly be accessed.
- **Stale handle:** an old reference that no longer identifies the current logical object.
- **Generation counter:** a version number associated with a slot, checked when a handle is used.
- **ABA-like problem:** a location appears unchanged, even though its logical contents were replaced in between.

The fixture uses a permanently allocated stack `Slot` to avoid dereferencing freed memory. `Handle` stores a pointer **and** the slot's generation. After `recycle()`, the old handle fails validation even though its pointer equals the new handle's pointer. It also refuses null outputs, inactive slots and generation overflow.

## Build, run, inspect
~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 generation_handles.c -o generation_handles
./generation_handles
gdb -q ./generation_handles
~~~
In GDB, break at `read_value`, compare `h.slot` and `h.generation` before/after recycling. In Ghidra, locate the `occupied` and generation checks before the final field read. Repeat with `-O2` to see optimized control flow.

## Exercises
1. Why isn't pointer equality sufficient to establish logical identity?
2. How does generation rollover affect a long-running allocator?
3. What would make this approach unsafe if the underlying `Slot` itself were freed?
4. Why do generation checks need synchronization in multi-threaded programs?
5. Where is the trust boundary between a handle and the object it references?

## Scope
This is **not** a general-use memory allocator or proof against use-after-free. Its slot stays alive throughout execution; the example only models logical invalidation and safe validation order.
