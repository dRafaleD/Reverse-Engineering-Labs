# Day 12 — malloc and Heap Basics

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Learn the basic idea of dynamic memory allocation in C and recognize the clues that `malloc()` and `free()` leave in a compiled binary.

## Source code

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *numbers = malloc(3 * sizeof(int));

    if (numbers == NULL) {
        return 1;
    }

    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;

    printf("%d %d %d\n", numbers[0], numbers[1], numbers[2]);

    free(numbers);
    return 0;
}
```

## What is the heap?

The heap is a memory area used for dynamically allocated data. Unlike a simple local variable with a fixed lifetime, heap memory is requested while the program is running.

In this example:

```c
malloc(3 * sizeof(int))
```

requests enough memory for three `int` values.

If an `int` is 4 bytes on the current platform, the requested size is usually:

```text
3 × 4 = 12 bytes
```

## What does `malloc()` return?

`malloc()` returns a pointer to the allocated memory.

So:

```c
int *numbers = malloc(...);
```

means that `numbers` stores an address.

If allocation fails, `malloc()` returns `NULL`, which is why the program checks:

```c
if (numbers == NULL)
```

## Array access and pointer arithmetic

The expressions:

```c
numbers[0]
numbers[1]
numbers[2]
```

refer to consecutive `int` values in the allocated block.

At a lower level, think in terms of:

```text
base address + index × element size
```

For 4-byte integers:

```text
numbers[0] -> base + 0
numbers[1] -> base + 4
numbers[2] -> base + 8
```

This connects directly to the array and pointer patterns from earlier labs.

## What to look for in Ghidra

Compile with debug information:

```bash
gcc -g heap.c -o heap
```

Then inspect `main` in Ghidra.

Useful clues include calls such as:

```text
CALL malloc
CALL printf
CALL free
```

You may also see:

- a comparison against zero after `malloc`
- memory writes using offsets from the returned pointer
- the pointer later being passed to `free`

The exact instructions depend on compiler and optimization settings, so focus on the overall pattern.

## Reverse engineering pattern

A useful mental model is:

```text
size prepared
    ↓
CALL malloc
    ↓
returned pointer
    ↓
NULL check
    ↓
writes through pointer + offsets
    ↓
CALL free
```

If you see this pattern in a binary, it often suggests dynamically allocated memory is being created, used, and released.

## Stack vs heap — beginner view

For now, keep the distinction simple:

```text
stack -> local function data, automatic lifetime
heap  -> dynamically requested memory, manually released in C
```

Real programs can be more complex, but this distinction is enough for the current lab.

## Main takeaway

```text
malloc(size) -> asks for heap memory
return value -> pointer/address
NULL         -> allocation failure indicator
[index]      -> address + index × element size
free(ptr)    -> releases allocated memory
```

The reverse engineering goal is not to memorize allocator internals yet. The goal is to recognize allocation, pointer-based memory access, and deallocation as one connected pattern.
