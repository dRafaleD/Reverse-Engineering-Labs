# Day 9 — Structs and Memory Layout

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Learn how a simple C `struct` groups several values together and how those fields can appear as fixed offsets in memory when inspected in Ghidra.

The goal is not to memorize compiler output. It is to recognize a common reverse-engineering pattern:

```text
base address + field offset
```

## Source Code

```c
#include <stdio.h>

struct Player {
    int health;
    int ammo;
    char rank;
};

int main() {
    struct Player player = {100, 30, 'A'};

    printf("Health: %d\n", player.health);
    printf("Ammo: %d\n", player.ammo);
    printf("Rank: %c\n", player.rank);

    return 0;
}
```

Compile without optimization for a clearer first look:

```bash
gcc -g -O0 structs.c -o structs
```

Then import the binary into Ghidra and inspect `main`.

## What Is a Struct?

A `struct` groups related values under one type.

```c
struct Player {
    int health;
    int ammo;
    char rank;
};
```

Conceptually, one `Player` object contains three fields:

```text
health
ammo
rank
```

Instead of three unrelated variables, the fields belong to the same object.

## Memory Layout Idea

The fields are stored at different offsets from the beginning of the struct.

A simplified mental model can look like this:

```text
Player base
+0x00 -> health
+0x04 -> ammo
+0x08 -> rank
```

The exact total size can include padding, so do not assume the struct size is simply the visible field sizes added together.

## Why Padding Exists

Compilers may insert unused bytes between fields or at the end of a struct to satisfy alignment requirements.

For example, this struct contains:

```text
int  -> commonly 4 bytes
int  -> commonly 4 bytes
char -> 1 byte
```

but `sizeof(struct Player)` may be larger than 9 bytes because of padding.

This matters in reverse engineering because a field may appear at a stable offset while the next object begins later than expected.

## What to Look for in Ghidra

You may see accesses similar to:

```asm
MOV dword ptr [RBP - 0x10], 0x64
MOV dword ptr [RBP - 0x0c], 0x1e
MOV byte ptr  [RBP - 0x08], 0x41
```

The exact offsets can differ, but the important pattern is that several values are stored near each other at predictable offsets.

Values used here:

```text
0x64 = 100
0x1e = 30
0x41 = 'A'
```

## Field Access Pattern

If a pointer to a struct is stored in a register, field access can look conceptually like:

```asm
MOV EAX, dword ptr [RDI + 0x4]
```

A useful RE interpretation is:

```text
RDI        -> address of an object
[RDI+0x4] -> one field inside that object
```

Repeated accesses such as:

```text
[base + 0x0]
[base + 0x4]
[base + 0x8]
```

can be a clue that the program is working with a structure or another grouped data layout.

## Ghidra and Reconstructed Structs

When symbols and debug information are available, Ghidra may already know useful names and types.

In stripped or unknown binaries, the original field names may be gone. A reverse engineer may first see only offsets and later infer that they belong to one object.

For example:

```text
object + 0x00 -> probably health
object + 0x04 -> probably ammo
object + 0x08 -> probably rank/state
```

Those names are hypotheses until supported by program behavior.

## Main Takeaway

```text
struct
  ↓
grouped fields
  ↓
fields stored at offsets
  ↓
base address + offset
  ↓
recognizable memory-access patterns in Ghidra
```

For Day 9, the important idea is simple: when several nearby offsets are repeatedly accessed from the same base address, they may represent fields of the same data structure.
