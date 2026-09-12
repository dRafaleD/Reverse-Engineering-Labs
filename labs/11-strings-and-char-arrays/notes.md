# Day 11 — Strings and Character Arrays

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Learn how simple C strings are represented as character arrays and what clues they leave inside a compiled binary.

## Source code

```c
#include <stdio.h>
#include <string.h>

int main() {
    char name[] = "Rafale";
    char code[] = "RE_LAB_11";

    printf("Name: %s\n", name);
    printf("Code: %s\n", code);
    printf("Name length: %zu\n", strlen(name));

    return 0;
}
```

## What is a C string?

In C, a string is usually stored as an array of `char` values ending with a null byte:

```text
R  a  f  a  l  e  \0
```

The final `\0` marks the end of the string.

So this:

```c
char name[] = "Rafale";
```

is not one single object at the machine level. It is a sequence of bytes stored next to each other in memory.

## Why this matters in reverse engineering

Readable strings are often some of the easiest clues to recognize in a binary. They can reveal:

- messages printed by the program
- file paths
- error text
- menu labels
- protocol names
- configuration values

A string alone does not explain program behavior, but references to that string can lead us to relevant code.

## What to look for in Ghidra

After compiling with debug information:

```bash
gcc -g strings.c -o strings
```

open the binary in Ghidra and inspect `main`.

Look for patterns such as:

```text
LEA  register, [address]
MOV  argument_register, register
CALL printf
```

You may also see a call to:

```text
strlen
```

The compiler can transform the exact instruction sequence, so focus on the pattern rather than expecting identical assembly every time.

## Strings window

Ghidra can list strings found in the binary. A useful path is:

```text
Window -> Defined Strings
```

or use string search features depending on the Ghidra layout/version.

If you find `RE_LAB_11`, following its XREFs can show where the program uses it.

## XREF reminder

`XREF` means cross-reference.

If a string has an XREF to `main`, that means code inside `main` refers to that string.

This creates a useful RE workflow:

```text
interesting string
      ↓
find XREF
      ↓
open referencing function
      ↓
inspect surrounding instructions
```

## `strlen()`

`strlen(name)` counts characters until it reaches the null terminator.

For:

```text
Rafale\0
```

`strlen()` returns `6` because the terminating null byte is not counted.

In the binary, an external function call may appear as:

```text
CALL strlen
```

This gives another clue about what the nearby data represents.

## Main reverse engineering connection

At source level:

```c
char name[] = "Rafale";
printf("%s", name);
```

At binary level, think more like:

```text
bytes containing characters
        ↓
address of those bytes
        ↓
address passed to a function
        ↓
printf / puts / strlen / another routine
```

The important idea is that strings become bytes and code works with their addresses.

## Main takeaway

```text
C string       -> char bytes ending in 00
string address -> pointer to the first character
XREF           -> code that refers to the string
CALL strlen    -> a clue that data is treated as text
```

Do not try to memorize every instruction yet. The goal is to recognize the relationship between readable strings, memory addresses, references, and function calls.
