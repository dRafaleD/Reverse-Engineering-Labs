# Day 5 — Stack Frames and Local Variables

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

The goal of this lab is to understand what happens on the stack while a function is running and why Ghidra often displays local variables using `RBP`- or `RSP`-based addresses.

We will connect:

- the stack
- `RSP`
- `RBP`
- local variables
- stack frames
- `PUSH`, `POP`, `SUB`, `ADD`, `LEAVE`, and `RET`

The goal is not to memorize the stack. It is to understand how a function creates temporary working space.

## Source Code

```c
#include <stdio.h>

int calculate(int a, int b) {
    int sum = a + b;
    int doubled = sum * 2;
    int result = doubled - 3;

    return result;
}

int main() {
    int x = 7;
    int y = 5;
    int output = calculate(x, y);

    printf("Result: %d\n", output);

    return 0;
}
```

Compile with debug information:

```bash
gcc -g stack_frame.c -o stack_frame
```

For a clearer stack frame during this lab:

```bash
gcc -g -O0 -fno-omit-frame-pointer stack_frame.c -o stack_frame
```

Import the binary into Ghidra and inspect `calculate`.

## What Is the Stack?

The stack is a memory region commonly used for temporary data during function calls.

It may contain:

- local variables
- some function arguments
- saved register values
- return addresses

On x86-64, the stack typically grows toward lower memory addresses.

## `RSP`

`RSP` is the stack pointer.

It tracks the current top of the stack.

Instructions such as `PUSH` and `POP` move it as data is added to or removed from the stack.

## `RBP`

In simple/debug builds, `RBP` is often used as a stable reference point for the current function's stack frame.

A common prologue is:

```asm
PUSH RBP
MOV  RBP, RSP
```

Local variables may then appear as:

```text
[RBP - 0x4]
[RBP - 0x8]
[RBP - 0xc]
```

These are different locations inside the current function's stack frame.

## Local Variables on the Stack

The C code:

```c
int sum = a + b;
int doubled = sum * 2;
int result = doubled - 3;
```

may leave traces similar to:

```asm
MOV dword ptr [RBP - 0x4], EAX
MOV dword ptr [RBP - 0x8], EAX
MOV dword ptr [RBP - 0xc], EAX
```

Each negative offset may correspond to a different local variable.

## Why Negative Offsets?

`RBP - 0x4` means a location four bytes below the frame-base reference.

A simplified view:

```text
RBP        -> frame reference
RBP - 0x4 -> local variable
RBP - 0x8 -> local variable
RBP - 0xc -> local variable
```

## `SUB RSP, ...`

A function may reserve stack space with:

```asm
SUB RSP, 0x20
```

Conceptually:

> Reserve 32 bytes of stack space for this function's temporary work.

## Cleaning Up the Frame

Near the end, you may see:

```asm
ADD RSP, 0x20
POP RBP
RET
```

or:

```asm
LEAVE
RET
```

These restore stack/frame state before returning to the caller.

## Simplified `calculate` Flow

```text
calculate starts
    ↓
old RBP is saved
    ↓
new stack frame is established
    ↓
a and b are processed
    ↓
sum may be stored on the stack
    ↓
doubled may be stored on the stack
    ↓
result may be stored on the stack
    ↓
return value is prepared in EAX
    ↓
stack frame is cleaned up
    ↓
RET
```

## What to Look for in Ghidra

Open `calculate` and compare Listing with Decompiler.

Try to locate:

1. `PUSH RBP` / `MOV RBP, RSP`
2. any `SUB RSP, ...`
3. accesses such as `[RBP - ...]`
4. the relationship between `sum`, `doubled`, and `result`
5. the function epilogue and `RET`

## Ghidra Local Variable Names

Decompiler may show names such as:

```c
int local_c;
int local_8;
int local_4;
```

These may be automatically generated names for stack variables.

That is normal. Reverse engineers often infer the meaning and rename them.

## Important: Optimized Binaries May Look Different

With compiler optimization enabled, some local variables may never be stored on the stack at all.

A value may remain entirely in a register.

That is why this lab uses:

```bash
-O0 -fno-omit-frame-pointer
```

A key lesson is:

> A variable in the source code does not guarantee a separate stack variable in the compiled binary.

## Mini Exercise

Open `calculate` in Ghidra and try to answer:

1. Which registers initially carry `a` and `b`?
2. What stack offset is used for the first local variable?
3. How many different `[RBP - ...]` accesses can you find?
4. Is `sum * 2` implemented with `MUL`, or did the compiler choose another instruction?
5. Can you locate the point where the return value reaches `EAX`?

## Main Takeaway

```text
function is called
      ↓
stack frame is prepared
      ↓
RBP may become a stable reference
      ↓
local variables may live in stack space
      ↓
RSP tracks the current top of the stack
      ↓
function finishes
      ↓
frame is cleaned up
      ↓
RET
```

After this lab, an expression such as `[RBP - 0x4]` should start to look less like a random address and more like a possible local variable inside the current function's stack frame.
