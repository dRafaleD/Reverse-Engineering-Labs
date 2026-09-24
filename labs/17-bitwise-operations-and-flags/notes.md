# Day 17 — Bitwise Operations and Flags

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Learn how bitwise operations are used to test and modify individual bits, and how those patterns can appear in a compiled binary.

## Source code

```c
#include <stdio.h>

int main(void) {
    unsigned int permissions = 0x5; // binary: 0101
    unsigned int write_flag = 0x2;  // binary: 0010

    if ((permissions & write_flag) != 0) {
        printf("Write permission is enabled\n");
    } else {
        printf("Write permission is disabled\n");
    }

    permissions |= write_flag;

    printf("Updated permissions: 0x%X\n", permissions);

    return 0;
}
```

## Why bitwise operations matter

Programs often pack multiple boolean values into one integer.

For example:

```text
bit 0 -> read
bit 1 -> write
bit 2 -> execute
```

The value:

```text
0101
```

means bit 0 and bit 2 are enabled.

## Bitwise AND

This expression:

```c
permissions & write_flag
```

tests whether the write bit is set.

Example:

```text
permissions = 0101
write_flag  = 0010
-------------------
AND         = 0000
```

The result is zero, so the bit is not enabled.

In assembly, a compiler may use instructions such as:

```text
AND
TEST
CMP
JE / JNE
```

A very common pattern is:

```text
TEST register, mask
JE   not_enabled
```

## Bitwise OR

This line:

```c
permissions |= write_flag;
```

sets the requested bit without clearing the others.

```text
0101
0010
----
0111
```

In assembly, this may appear as:

```text
OR register, immediate
```

or as a load-modify-store sequence when the value is in memory.

## Masks

A value used to select particular bits is often called a **mask**.

Examples:

```text
0x1 -> 0001
0x2 -> 0010
0x4 -> 0100
0x8 -> 1000
```

When reverse engineering, repeated values such as `1`, `2`, `4`, `8`, `0x10`, and so on can be clues that code is working with flags.

## Ghidra workflow

Compile:

```bash
gcc -g bitwise.c -o bitwise
```

Open the binary in Ghidra and inspect `main`.

Look for:

- constants such as `0x5` and `0x2`
- `AND`, `OR`, or `TEST`
- a conditional jump after the flag check
- the branch that prints enabled or disabled

Do not expect the compiler to produce exactly the same instructions every time.

## Main reverse engineering connection

At source level:

```c
if ((permissions & write_flag) != 0)
```

At machine level, think:

```text
value
  ↓
apply bit mask
  ↓
check whether result is zero
  ↓
conditional jump
```

This pattern appears in:

- permission fields
- status flags
- feature flags
- file format flags
- protocol fields
- configuration options

## Main takeaway

```text
AND  -> test/keep selected bits
OR   -> set selected bits
mask -> value describing which bits matter
TEST -> common flag-check instruction
```

The goal is not to memorize every hexadecimal mask yet. Learn to recognize the pattern of **mask -> test -> branch**.
