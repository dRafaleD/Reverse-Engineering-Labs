# Day 4 — C Functions and Calling Conventions

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

The goal of this lab is to understand how a simple C function appears in Ghidra after compilation.

We will connect:

- C function parameters
- x86-64 registers
- the `CALL` instruction
- function return values
- stack frames
- the Linux x86-64 System V calling convention

The goal is not to memorize the entire calling convention. It is to recognize the traces a function call leaves inside a binary.

## Source Code

```c
#include <stdio.h>

int add(int a, int b) {
    int result = a + b;
    return result;
}

int main() {
    int x = 10;
    int y = 20;

    int total = add(x, y);

    printf("Result: %d\n", total);

    return 0;
}
```

Compile with debug information:

```bash
gcc -g functions.c -o functions
```

Import the resulting binary into Ghidra and inspect `main` and `add`.

## What Happens at the C Level?

```c
int total = add(x, y);
```

At a high level:

```text
x = 10
y = 20

add(10, 20)
    ↓
returns 30
    ↓
total = 30
```

The CPU does not understand high-level concepts such as "pass two numbers to a function." The compiler turns that operation into register transfers and instructions.

## What Is a Calling Convention?

A calling convention defines how functions communicate.

It answers questions such as:

- Which registers carry arguments?
- Where is the return value placed?
- How is the stack used?
- Which registers must be preserved?

On Linux x86-64 using the System V ABI, the first integer/pointer arguments are commonly passed in:

```text
1st argument -> RDI
2nd argument -> RSI
3rd argument -> RDX
4th argument -> RCX
5th argument -> R8
6th argument -> R9
```

For 32-bit `int` values, the lower 32-bit register names may be used:

```text
RDI -> EDI
RSI -> ESI
```

For this lab, the first two are enough.

## `main` in Ghidra

The exact output depends on compiler version and settings, but the logic may look similar to:

```asm
MOV dword ptr [RBP - 0x4], 0xa
MOV dword ptr [RBP - 0x8], 0x14

MOV EDX, dword ptr [RBP - 0x8]
MOV EAX, dword ptr [RBP - 0x4]

MOV ESI, EDX
MOV EDI, EAX

CALL add

MOV dword ptr [RBP - 0xc], EAX
```

### `0xa` and `0x14`

```text
0x0a = 10
0x14 = 20
```

These correspond to:

```c
int x = 10;
int y = 20;
```

## Preparing the Arguments

Immediately before the function call we may see instructions such as:

```asm
MOV ESI, EDX
MOV EDI, EAX
CALL add
```

The important connection is:

```text
EDI -> first int argument of add
ESI -> second int argument of add
```

So:

```c
add(x, y);
```

roughly becomes:

```text
EDI = x
ESI = y
CALL add
```

## `CALL add`

```asm
CALL add
```

`CALL` transfers execution to another function and also preserves the return address so execution can continue afterward.

Simplified flow:

```text
main
  |
  | EDI = 10
  | ESI = 20
  |
  +---- CALL add ----+
                     |
                     v
                    add
                     |
                     | 10 + 20
                     |
                     v
                   return
                     |
                     +----> main continues
```

## Inside `add`

A simplified version may look like:

```asm
PUSH RBP
MOV  RBP, RSP

MOV dword ptr [RBP - 0x14], EDI
MOV dword ptr [RBP - 0x18], ESI

MOV EDX, dword ptr [RBP - 0x14]
MOV EAX, dword ptr [RBP - 0x18]

ADD EAX, EDX

MOV dword ptr [RBP - 0x4], EAX
MOV EAX, dword ptr [RBP - 0x4]

POP RBP
RET
```

The exact instruction sequence can vary with compiler version and optimization settings.

The important part is the pattern.

## `ADD`

```asm
ADD EAX, EDX
```

Conceptually:

```text
EAX = EAX + EDX
```

This is one assembly-level trace of:

```c
int result = a + b;
```

## Function Return Value

Integer return values are commonly returned through `EAX` / `RAX` under the System V ABI.

So:

```c
return result;
```

leaves the result in `EAX` before the function returns.

After `CALL add`, `main` may contain something like:

```asm
MOV dword ptr [RBP - 0xc], EAX
```

Conceptually:

```text
add(10, 20)
    ↓
EAX = 30
    ↓
total = EAX
```

## Why Do We See the Stack Frame Again?

At the beginning:

```asm
PUSH RBP
MOV RBP, RSP
```

and near the end:

```asm
POP RBP
RET
```

These instructions are related to setting up and cleaning up the function's stack frame.

The pattern from Day 2 now has more context:

```text
function starts
    ↓
creates space/reference for local work
    ↓
performs its logic
    ↓
restores previous state
    ↓
returns to caller
```

## What Does the Ghidra Decompiler Show?

The decompiler may reconstruct something similar to:

```c
int add(int param_1, int param_2)
{
    int local_var;

    local_var = param_1 + param_2;
    return local_var;
}
```

With debug symbols, names may be more readable.

Remember:

> Ghidra does not recover the original C source code.

It builds a high-level representation from machine code.

Names such as:

```text
a        -> param_1
b        -> param_2
result   -> local_10
```

may therefore appear.

## Reading Listing and Decompiler Together

Decompiler:

```c
return param_1 + param_2;
```

Listing:

```asm
MOV ...
ADD ...
MOV ...
RET
```

The decompiler helps answer "what is this code doing?"

The Listing view helps answer "how is the CPU doing it?"

Using both together is much more useful than relying on only one.

## Day 4 Mini Exercise

Open `main` and `add` in Ghidra and try to find:

1. Which hexadecimal values represent `10` and `20`?
2. Which registers are prepared immediately before `CALL add`?
3. Which register carries the return value from `add`?
4. Can you locate the `ADD` instruction?
5. Which decompiler statements can you match to instructions in the Listing?

Try to trace them in Ghidra instead of memorizing the answers.

## Main Takeaway

```text
C function call
        ↓
arguments are prepared in registers
        ↓
CALL
        ↓
function performs its work
        ↓
result returns through EAX/RAX
        ↓
RET
        ↓
caller continues
```

After this lab, `CALL`, `RDI/EDI`, `RSI/ESI`, `RAX/EAX`, `RBP`, and `RET` should begin to look like connected parts of a function call rather than unrelated assembly terms.
