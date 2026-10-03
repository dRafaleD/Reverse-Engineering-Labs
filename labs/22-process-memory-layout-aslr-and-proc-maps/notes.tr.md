# Gün 22 — Process Memory Layout, Virtual Memory, ASLR ve /proc Maps

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Connect the static ELF view from Day 20 and the GDB runtime view from Day 21 to the **memory map of a live Linux process**.

This day combines several tightly related concepts:

1. virtual memory,
2. process address spaces,
3. executable mappings,
4. stack and heap,
5. shared libraries,
6. memory permissions,
7. ASLR,
8. PIE vs non-PIE,
9. `/proc/<pid>/maps`,
10. GDB memory mappings.

The goal is not to memorize addresses. It is to understand **what kinds of regions exist, why they exist, and how static binary structures become runtime mappings**.

## 1. File on disk vs process in memory

An ELF file on disk is not identical to the process you see at runtime.

```text
ELF file
  sections + program headers
          ↓ loader
virtual address space
  executable mappings
  data mappings
  heap
  shared libraries
  stack
  kernel-provided regions
```

Day 20 inspected the file.

Day 21 paused execution.

Day 22 asks:

> Where did everything go in memory?

## 2. Virtual memory

A process normally works with **virtual addresses**.

The operating system and hardware translate those virtual addresses to underlying physical memory/storage mappings as needed.

Important beginner distinction:

```text
virtual address != physical RAM address
```

Two processes can use similar virtual addresses while remaining isolated by separate address spaces.

## 3. Why virtual memory matters to RE

Reverse engineering constantly encounters addresses:

- disassembler addresses,
- debugger addresses,
- pointers,
- stack addresses,
- heap pointers,
- shared-library addresses.

Without a memory-layout model, these numbers look arbitrary.

With a model, you ask:

> Which mapping contains this address?

That question is much more useful.

## 4. Training program

Compile:

```bash
gcc -g -O0 source/memory_map.c -o memory_map
```

Run:

```bash
./memory_map
```

The program prints:

- PID,
- address of `main`,
- address of a global,
- address of a stack local,
- address returned by `malloc`.

Then it waits for Enter so you have time to inspect its process mappings.

## 5. Inspect /proc/<pid>/maps

While the program waits, use another terminal:

```bash
cat /proc/<PID>/maps
```

Replace `<PID>` with the printed process ID.

Typical lines contain:

```text
address-range permissions offset device inode pathname
```

Example shape:

```text
5555...-5555... r-xp ... /path/memory_map
7f...-7f...       r-xp ... libc.so...
7fff...-7fff...   rw-p ... [stack]
```

Exact addresses vary.

## 6. Memory permissions

Common letters:

- `r` — readable
- `w` — writable
- `x` — executable
- `p` — private mapping

Ask why a region needs each permission.

Conceptually:

```text
machine code -> read + execute
mutable data -> read + write
stack        -> read + write
```

Modern systems try to avoid writable+executable memory unless there is a reason.

This connects memory layout to exploit mitigations without turning this lab into exploitation training.

## 7. Executable mappings

Find the lines containing:

```text
memory_map
```

There may be multiple mappings for the same ELF file with different permissions/offsets.

Why?

The loader maps different portions according to runtime needs.

This is where Day 20's distinction becomes important:

```text
sections -> linker/static-analysis organization
segments -> loader/runtime mapping view
```

Sections and memory mappings are related, but not identical concepts.

## 8. Program headers

Inspect:

```bash
readelf -l memory_map
```

Compare the `LOAD` program headers with:

```bash
cat /proc/<PID>/maps
```

You are connecting:

```text
ELF program headers
        ↓
loader
        ↓
runtime mappings
```

This is more useful for runtime understanding than memorizing every section name.

## 9. Stack

The local variable:

```c
int stack_value = 21;
```

usually lives in the active stack frame in this unoptimized build.

The program prints:

```text
&stack_value=...
```

Find the `[stack]` mapping and check whether the address falls inside its range.

That gives a concrete test:

```text
local variable address
        ↓
inside [stack] range?
```

## 10. Heap

The program calls:

```c
malloc(64)
```

and prints the returned pointer.

Find `[heap]` if it appears in the process maps and compare the range.

Important nuance:

> Not every allocation on every libc/system must come from one visible traditional `[heap]` region.

Allocators can use different mechanisms, including anonymous mappings.

For this beginner example, focus on observing what your own environment actually does.

## 11. Global data

The program contains:

```c
int global_counter = 7;
```

Its address should normally fall within a writable mapping associated with the executable.

Connect this to Day 20:

```text
initialized global
      ↓
.data concept
      ↓
writable runtime mapping
```

Again, section names and mappings are not a one-to-one identity, but the relationship becomes visible.

## 12. Read-only data

The source contains:

```c
static const char banner[] = "RE_DAY22_MEMORY_MAP";
```

Use GDB:

```gdb
break main
run
print &banner
```

Then compare its address with mappings.

The constant is expected to be associated with read-only data in the compiled binary.

Use:

```bash
readelf -S memory_map
objdump -s -j .rodata memory_map
```

to connect static and runtime views.

## 13. Shared libraries

The process map should contain libraries such as libc.

The executable calls functions including:

- `printf`,
- `malloc`,
- `free`,
- `getpid`.

Earlier Day 18 introduced library calls and PLT/GOT.

Now observe where the actual shared library is mapped in the process.

```text
call site in executable
      ↓
dynamic linking machinery
      ↓
shared library mapping
```

## 14. ASLR

ASLR means Address Space Layout Randomization.

Its purpose is to make important memory locations less predictable across executions.

Run the program several times:

```bash
./memory_map
./memory_map
./memory_map
```

Record:

- `&main`
- stack variable address
- heap pointer

Compare them.

On a normal ASLR-enabled system, some addresses should vary.

## 15. PIE and ASLR

Modern compilers commonly produce PIE executables by default.

Check:

```bash
readelf -h memory_map | grep Type
```

You may see:

```text
DYN
```

for a PIE executable.

Build a non-PIE comparison:

```bash
gcc -g -O0 -no-pie source/memory_map.c -o memory_map_nopie
```

Run it multiple times and compare `&main`.

This demonstrates an important idea:

```text
ASLR + PIE
   ↓
main executable can be relocated

non-PIE
   ↓
main executable code address is commonly much more stable
```

Stack/shared-library addresses may still be randomized.

## 16. Do not disable ASLR yet

You may find tutorials that globally disable ASLR.

Do not do that for this lab.

The learning goal is to **observe the mitigation**, not remove it.

Changing global kernel security settings is unnecessary for understanding the concept.

## 17. GDB mapping view

Start:

```bash
gdb ./memory_map
```

Then:

```gdb
break main
run
info proc mappings
```

Depending on environment/GDB support, this displays the process mappings from inside the debugger.

Also inspect:

```gdb
print &main
print &global_counter
print &stack_value
```

After stepping past `malloc`:

```gdb
print heap_message
```

Now map each address to a region.

## 18. Address classification challenge

For each address, classify it before checking:

```text
&main            -> executable code mapping
&global_counter  -> writable executable data mapping
&stack_value     -> stack
heap_message     -> heap/allocator-managed writable memory
libc function    -> shared-library mapping
```

Then verify with `/proc/<pid>/maps` or GDB.

Prediction before verification is a powerful RE habit.

## 19. Static vs runtime addresses

Ghidra may show addresses based on the binary's image base/layout.

Runtime addresses can differ due to relocation and ASLR.

Therefore:

```text
static address
     != always
runtime absolute address
```

Instead, reason with:

- module base,
- offsets,
- symbol relationships,
- mapping ranges.

This becomes increasingly important in real dynamic analysis.

## 20. Memory map permissions exercise

For each mapping category, record expected permissions:

| Region | Typical idea |
| --- | --- |
| executable code | r-x |
| read-only constants | r-- |
| writable data | rw- |
| stack | rw- |
| heap | rw- |
| shared-library code | r-x |

These are common patterns, not universal laws.

Your actual process map is the evidence.

## 21. /proc maps vs smaps

Linux also exposes more detailed mapping information:

```bash
cat /proc/<PID>/smaps
```

It is much larger.

For now, notice that it adds memory-accounting details such as sizes and page statistics.

Do not try to memorize it all.

The key progression is:

```text
maps -> where and with what permissions?
smaps -> more detail about those mappings
```

## 22. Mini challenge: explain five addresses

Run the program and create a small table:

```text
Address | Object | Mapping | Permissions | Why?
```

Include:

1. `main`,
2. `global_counter`,
3. `stack_value`,
4. `heap_message`,
5. one libc mapping.

For each one, explain **why** it belongs there.

## 23. Mini challenge: PIE comparison

Build:

```bash
gcc -g -O0 source/memory_map.c -o memory_map
gcc -g -O0 -no-pie source/memory_map.c -o memory_map_nopie
```

Run each three times.

Record `&main`.

Answer:

- Which one changes more?
- Which other regions still move?
- What does this teach you about PIE and ASLR?
- Why should a reverse engineer avoid hardcoding runtime absolute addresses?

## 24. Reverse-engineering workflow

```text
inspect ELF
    ↓
read program headers
    ↓
run process
    ↓
inspect /proc maps
    ↓
classify addresses
    ↓
verify with GDB
    ↓
compare PIE/non-PIE
    ↓
connect static and runtime views
```

## Exercises

1. Compile the default build.
2. Record the PID and printed addresses.
3. Inspect `/proc/<PID>/maps`.
4. Find the executable mappings.
5. Find the stack mapping.
6. Find the heap/allocation region.
7. Find libc.
8. Compare `readelf -l` with runtime mappings.
9. Use GDB `info proc mappings`.
10. Build a non-PIE version.
11. Run both versions multiple times.
12. Complete the five-address challenge.
13. Explain why sections and segments are different.
14. Explain why absolute runtime addresses are fragile.

## Questions

1. Virtual vs physical address?
2. What is a process address space?
3. Why can one ELF produce multiple mappings?
4. What do r/w/x permissions mean?
5. Where would a stack local normally appear?
6. Where might `malloc` memory appear?
7. Why is libc mapped into the process?
8. What does ASLR randomize?
9. How does PIE interact with ASLR?
10. Why is `main` not guaranteed to have the same absolute address?
11. Sections vs segments?
12. Why is `/proc/<pid>/maps` useful in RE?
13. What does `smaps` add conceptually?
14. Why should observation beat address memorization?

## Main takeaway

```text
ELF on disk
   ↓
program headers
   ↓
loader
   ↓
virtual memory mappings
   ↓
code + data + heap + libraries + stack
   ↓
ASLR changes locations
   ↓
GDB verifies the live process
```

Day 22 connects the binary you inspect statically to the address space you debug dynamically.


> Türkçe çalışma notu: Bu günün ana hedefi adres ezberlemek değil, bir runtime address gördüğünde **hangi mapping içinde ve neden orada?** sorusunu cevaplayabilmek. Özellikle Day 20'deki ELF section/segment bilgisi ile Day 21'deki GDB kullanımını birlikte uygula.
