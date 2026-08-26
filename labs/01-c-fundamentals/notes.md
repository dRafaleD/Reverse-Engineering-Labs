# Day 1 — C Fundamentals

## Goal

The goal of this lab is to understand the basic structure of a C program and compile a simple executable that can later be inspected during reverse engineering exercises.

## Source Code

The source file for this lab is located at:

`source/hello.c`

```c
#include <stdio.h>

int main() {
    printf("This is my first c code\n");
    return 0;
}
```

## What I Learned

### `#include <stdio.h>`

Includes the Standard Input/Output header file. It provides declarations for functions such as `printf()` and `scanf()`.

### `int main()`

`main` is the entry point of the C program. Program execution starts from this function.

The `int` means that the function returns an integer value to the operating system.

### `printf()`

`printf()` prints formatted text to standard output.

```c
printf("This is my first c code\n");
```

The `\n` character creates a new line after the text is printed.

### `return 0;`

Ends the `main` function and returns `0` to the operating system. A return value of `0` normally indicates that the program finished successfully.

## Compilation

Compile the program with GCC:

```bash
gcc hello.c -o hello
```

Run the executable:

```bash
./hello
```

Expected output:

```text
This is my first c code
```

## Reverse Engineering Connection

This is the first step toward understanding how source code becomes a binary executable.

The basic flow is:

```text
C source code
    ↓
Compiler (GCC)
    ↓
ELF executable
    ↓
Disassembler / Decompiler (for example Ghidra)
```

Later labs will inspect how C concepts such as variables, conditions, functions and loops appear inside compiled binaries.

## Notes

For this first lab, the important concepts are:

- Basic C program structure
- `stdio.h`
- `main()`
- `printf()`
- `return 0`
- Compiling with GCC
- Running a Linux executable
