# Day 10 — Switch Statements and Jump Tables

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Understand how a C `switch` statement can appear in a compiled binary and learn the basic idea behind compare/jump chains and jump tables.

## Source code

```c
#include <stdio.h>

int main() {
    int choice = 4;

    switch (choice) {
        case 1:
            printf("Option 1\n");
            break;
        case 2:
            printf("Option 2\n");
            break;
        case 3:
            printf("Option 3\n");
            break;
        case 4:
            printf("Option 4\n");
            break;
        case 5:
            printf("Option 5\n");
            break;
        case 6:
            printf("Option 6\n");
            break;
        default:
            printf("Unknown option\n");
            break;
    }

    return 0;
}
```

## Compile

A normal debug build:

```bash
gcc -g switch.c -o switch
```

An optimized build is also useful for comparison:

```bash
gcc -O2 -g switch.c -o switch_O2
```

Open both binaries in Ghidra and compare `main`.

## What does a switch do?

At source level, a `switch` chooses one path depending on the value of a variable.

```text
choice = 4
    ↓
switch(choice)
    ↓
case 4
```

The compiler does not have to preserve the original `switch` syntax. It only needs to preserve the program behavior.

## Pattern 1 — Compare and jump chain

One possible compiled form is a series of comparisons:

```asm
CMP value, 1
JE  case_1
CMP value, 2
JE  case_2
CMP value, 3
JE  case_3
...
JMP default
```

This is conceptually similar to several `if / else if` checks.

Important instructions to recognize:

```text
CMP  -> compare values
JE   -> jump if equal
JNE  -> jump if not equal
JMP  -> unconditional jump
```

## Pattern 2 — Jump table

For a dense group of case values, especially with optimization enabled, a compiler may use a jump table instead of comparing every case one by one.

The idea is roughly:

```text
case number
    ↓
range check
    ↓
use value as an index
    ↓
read target address from a table
    ↓
jump to that case block
```

A simplified mental model:

```text
jump_table[0] -> case 1
jump_table[1] -> case 2
jump_table[2] -> case 3
jump_table[3] -> case 4
```

The exact assembly depends on compiler version, architecture, optimization level, and the source code. A `switch` does not always become a jump table.

## What to look for in Ghidra

When inspecting the function, look for:

- multiple conditional branches going to different blocks,
- a range check before the case logic,
- an indexed memory access,
- an indirect jump,
- several blocks that later merge into one common exit path.

Ghidra's decompiler may reconstruct these branches back into a readable `switch`, even though the Listing view shows lower-level jumps and addresses.

## Why this matters in reverse engineering

When source code is unavailable, recognizing a switch-like control-flow pattern can help reveal:

- command handlers,
- menu logic,
- state machines,
- protocol message types,
- enum-based decisions,
- opcode dispatch logic.

The important skill is not memorizing one exact assembly sequence. It is recognizing that one input value can select one of several possible code paths.

## Main takeaway

```text
C switch
   ↓
compiler
   ↓
compare/jump chain OR indexed table
   ↓
multiple control-flow paths
```

A decompiler may show a clean `switch`, but the underlying binary may implement it in a very different way.
