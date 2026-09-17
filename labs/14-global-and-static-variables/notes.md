# Day 14 — Global and Static Variables

## Goal

Learn how global and static variables differ from local stack variables and how they can appear in a compiled binary.

## Source Code

The example in `source/globals.c` contains a global variable, a static global variable, and a static local variable.

## Key Ideas

- Global variables have storage that exists for the lifetime of the program.
- `static` changes linkage or storage duration depending on where it is used.
- These variables are commonly stored outside the normal stack frame.
- Initialized global/static data commonly appears in sections such as `.data`.
- Zero-initialized global/static data commonly belongs to `.bss`.
- In assembly, accesses may use RIP-relative addressing on x86-64 Linux.

## Ghidra Practice

1. Compile the source with debug information:

```bash
gcc -g globals.c -o globals
```

2. Open the binary in Ghidra.
3. Find `main` and the helper function.
4. Look for references to the global and static variables.
5. Compare these accesses with the local variable accesses learned in earlier labs.

## Beginner RE Pattern

Local variable:

```text
[RBP - offset]
```

Global/static data on x86-64 may look like:

```text
[RIP + offset]
```

The exact instruction sequence depends on the compiler and build options, so the goal is to recognize the idea rather than memorize one exact listing.

## Takeaway

When reversing a program, variable location can provide useful clues about its lifetime and storage. Stack-relative accesses often point to local variables, while references to data outside the stack frame can indicate global or static storage.
