# Day 8 — Arrays and Pointer Basics

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

The goal of this lab is to understand the basic relationship between arrays, pointers, addresses, and indexed memory access before looking at the compiled code in Ghidra.

## Source Code

```c
#include <stdio.h>

int main() {
    int numbers[3] = {10, 20, 30};
    int *ptr = numbers;

    printf("First: %d\n", numbers[0]);
    printf("Second: %d\n", *(ptr + 1));
    printf("Third: %d\n", *(ptr + 2));

    return 0;
}
```

Compile with debug information:

```bash
gcc -g -O0 arrays_pointers.c -o arrays_pointers
```

## Array Basics

```c
int numbers[3] = {10, 20, 30};
```

creates three consecutive `int` values in memory.

On a typical system where `int` is 4 bytes, they are laid out conceptually like this:

```text
numbers[0] -> 10
numbers[1] -> 20
numbers[2] -> 30
```

The important reverse engineering idea is that an array is stored as neighboring elements in memory.

## Pointer Basics

```c
int *ptr = numbers;
```

`ptr` stores the address of the first element of the array.

For this lab, think of a pointer simply as:

```text
pointer = an address that tells us where some data is located
```

## Pointer Arithmetic

```c
*(ptr + 1)
```

means:

1. start from the address in `ptr`
2. move to the next `int`
3. read the value stored there

Because `ptr` is an `int *`, adding `1` moves by one integer element, not one raw byte.

So these expressions refer to the same values:

```c
numbers[0] == *(ptr + 0)
numbers[1] == *(ptr + 1)
numbers[2] == *(ptr + 2)
```

## What to Look for in Ghidra

After importing the binary, inspect `main` and look for patterns related to:

- local array storage on the stack
- addresses based on `RBP` or `RSP`
- `LEA` instructions used to calculate addresses
- memory reads using an address plus an offset
- multiplication/scaling related to element size

A common assembly-style memory expression may look like:

```asm
[RAX + RCX*4]
```

The `*4` can be a clue that the program is indexing 4-byte elements such as `int` values.

The exact registers and instructions may differ depending on compiler version and settings.

## Reverse Engineering Connection

In source code, arrays look simple:

```c
numbers[i]
```

At machine-code level, this becomes address calculation:

```text
base address
+
index × element size
=
address of the selected element
```

This pattern appears constantly in reverse engineering when analyzing arrays, tables, buffers, and structures.

## Main Takeaway

Do not try to memorize pointer syntax and assembly at the same time.

For now, remember these three ideas:

```text
array   -> consecutive elements in memory
pointer -> stores an address
index   -> address calculation based on element size
```

The goal of Day 8 is to start recognizing memory-address patterns in Ghidra.
