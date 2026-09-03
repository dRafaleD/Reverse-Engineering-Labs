# Day 7 — Loops and Backward Jumps

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

The goal of this lab is to understand how a simple C loop appears inside a compiled binary and how repeated execution is represented with comparisons and jumps.

We will connect:

- `for` loops
- loop counters
- `CMP`
- conditional jumps
- backward jumps
- repeated control flow

The goal is not to memorize every jump instruction. It is to recognize the basic shape of a loop in assembly and Ghidra.

## Source Code

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 5; i++) {
        printf("Iteration: %d\n", i);
    }

    return 0;
}
```

Compile without optimization so the structure is easier to inspect:

```bash
gcc -g -O0 loops.c -o loops
```

Import the binary into Ghidra and inspect `main`.

## What Happens at the C Level?

The loop can be separated into four simple steps:

```text
1. i = 0
2. check: i < 5 ?
3. run the loop body
4. i++ and go back to the check
```

This repeats until the condition becomes false.

## What Does a Loop Look Like in Assembly?

The exact output depends on compiler and settings, but a simple loop may look similar to:

```asm
MOV  dword ptr [RBP - 0x4], 0

loop_check:
CMP  dword ptr [RBP - 0x4], 4
JG   loop_end

; loop body
CALL printf

ADD  dword ptr [RBP - 0x4], 1
JMP  loop_check

loop_end:
```

The important part is the control flow, not the exact addresses or instruction choices.

## `CMP`

`CMP` compares two values.

For example:

```asm
CMP dword ptr [RBP - 0x4], 4
```

This may be part of checking whether the loop counter has reached its limit.

`CMP` does not store a normal result such as `true` or `false`. It updates CPU flags, and a jump instruction uses those flags.

## Conditional Jump

After the comparison, the program may use a conditional jump such as:

```asm
JG loop_end
```

This means execution jumps to `loop_end` when the comparison satisfies the required condition.

The exact conditional jump may differ depending on how the compiler transforms the original C condition.

## The Backward Jump

One of the easiest loop patterns to recognize is a jump that goes back to an earlier instruction:

```asm
JMP loop_check
```

Conceptually:

```text
check condition
    ↓
run body
    ↓
increment counter
    ↓
jump backward
    └──────────→ check condition again
```

This backward edge in control flow is a strong clue that you may be looking at a loop.

## Incrementing the Counter

The C code:

```c
i++;
```

may appear in different forms, such as:

```asm
ADD dword ptr [RBP - 0x4], 1
```

or another equivalent instruction sequence.

Again, the compiler does not have to preserve the exact syntax of the C source.

## What to Look for in Ghidra

When trying to identify a basic loop, look for this pattern:

```text
initialize a value
    ↓
compare it
    ↓
conditional jump out of the loop
    ↓
loop body
    ↓
modify the counter/state
    ↓
jump back to the comparison
```

In Ghidra's graph view, this may also appear as a control-flow arrow returning to an earlier basic block.

## `for` vs `while`

At the C level these look different:

```c
for (int i = 0; i < 5; i++) {
    // ...
}
```

```c
int i = 0;
while (i < 5) {
    // ...
    i++;
}
```

But after compilation, they can produce very similar control-flow structures.

This is another important reverse engineering lesson:

> Different source-code constructs can compile into very similar machine code.

## Main Takeaway

A loop is not stored in the binary as the word `for` or `while`.

Instead, you usually reconstruct it from a pattern of:

```text
initialization
    ↓
comparison
    ↓
conditional jump
    ↓
body
    ↓
state update
    ↓
backward jump
```

For now, being able to recognize that pattern is more important than memorizing jump mnemonics.
