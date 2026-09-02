# Day 6 — Conditions and Conditional Jumps

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

The goal of this lab is to understand how a simple C `if/else` statement appears after compilation and how Ghidra represents decision-making inside a binary.

We will connect:

- `if` / `else`
- comparisons
- CPU flags
- `CMP`
- conditional jump instructions
- control flow in Ghidra

The goal is not to memorize every jump instruction. It is to understand that a high-level condition becomes a comparison followed by a branch.

## Source Code

```c
#include <stdio.h>

int main() {
    int score = 72;

    if (score >= 50) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    return 0;
}
```

Compile with debug information and no optimization so the control flow is easier to inspect:

```bash
gcc -g -O0 conditions.c -o conditions
```

Import the resulting ELF binary into Ghidra and inspect `main`.

## What Happens at the C Level?

The logic is simple:

```text
score = 72
    ↓
is score >= 50?
    ↓
yes  -> print "Passed"
no   -> print "Failed"
```

The CPU does not directly understand the C keyword `if`. The compiler translates the condition into machine instructions that compare values and decide which address should execute next.

## `CMP`

A common instruction for comparisons is:

```asm
CMP destination, source
```

`CMP` performs a subtraction-like comparison internally, but it does not store the subtraction result as a normal value.

Instead, it updates CPU status flags.

For example, Ghidra may show something similar to:

```asm
CMP dword ptr [RBP - 0x4], 0x31
```

`0x31` is hexadecimal for decimal `49`.

Depending on how the compiler structures the branch, comparing against `49` can be used to implement the C condition `score >= 50` by jumping to the failure path when the value is `<= 49`.

The exact instructions can vary between compiler versions and settings, so focus on the logic rather than expecting one exact sequence.

## Conditional Jumps

After a comparison, the CPU can branch depending on the flags created by `CMP`.

Common instructions include:

```text
JE   -> jump if equal
JNE  -> jump if not equal
JG   -> jump if greater (signed)
JGE  -> jump if greater or equal (signed)
JL   -> jump if less (signed)
JLE  -> jump if less or equal (signed)
JA   -> jump if above (unsigned)
JB   -> jump if below (unsigned)
```

For this lab, do not memorize the whole list. The important pattern is:

```text
CMP
 ↓
conditional jump
 ↓
choose one code path
```

## Why Signed and Unsigned Jumps Exist

C integer values may be signed or unsigned.

For example:

```c
int x;
unsigned int y;
```

These values can require different interpretations during comparisons. This is why x86 has jump instructions such as `JG/JL` for signed comparisons and `JA/JB` for unsigned comparisons.

We will revisit this later. For now, our `score` variable is a normal signed `int`.

## Control Flow

An `if/else` statement creates more than one possible execution path.

A simplified assembly-like representation might look like:

```asm
CMP score, 50
JL  failed

; Passed path
CALL puts
JMP end_if

failed:
; Failed path
CALL puts

end_if:
MOV EAX, 0
RET
```

This is not guaranteed to match the compiler output exactly, but it shows the idea clearly.

The unconditional `JMP` instruction means:

> Continue execution from another address without checking a condition.

It is often used to skip the `else` block after the `if` block has already executed.

## Labels in Ghidra

Ghidra may give branch destinations names such as:

```text
LAB_00101180
LAB_00101190
```

These labels are not variable names from the original C source. They are names Ghidra creates for useful code locations and jump targets.

When you see:

```asm
JLE LAB_00101190
```

read it approximately as:

> If this condition is true, continue execution at `LAB_00101190`.

## Graph View

Conditions are easier to understand in Ghidra's graph view because each branch becomes a separate block.

Instead of reading only from top to bottom, think in terms of:

```text
        comparison
        /        \
     true        false
      |            |
   Passed        Failed
      \            /
          continue
```

This is called **control flow**: the order and possible paths in which instructions can execute.

## `CALL puts` Again

Just like the earlier labs, a simple call such as:

```c
printf("Passed\n");
```

may be compiled as:

```asm
CALL puts
```

The important lesson remains the same: source code and compiled output are logically related, but they are not always a line-by-line match.

## A Simple Reverse Engineering Strategy

When you see an unknown function containing branches, start simple:

1. Find comparisons such as `CMP` or `TEST`.
2. Look at the conditional jump immediately around them.
3. Follow both possible destinations.
4. Look for meaningful calls, strings, or return values in each branch.
5. Reconstruct the high-level decision.

For example:

```text
compare score
    ↓
branch A -> "Passed"
branch B -> "Failed"
```

From this, you can infer that the program is making a decision based on `score` even without having the original C source.

## Main Takeaway

Day 6 introduces one of the most important reverse engineering patterns:

```text
C condition
    ↓
comparison instruction
    ↓
CPU flags
    ↓
conditional jump
    ↓
different execution paths
```

You do not need to memorize every flag or jump yet.

For now, remember:

- `CMP` compares values.
- conditional jumps choose a path.
- `JMP` changes execution unconditionally.
- Ghidra labels help identify branch destinations.
- `if/else` becomes control flow inside the binary.
