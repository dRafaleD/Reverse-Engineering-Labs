# Day 15 — Signed and Unsigned Integers

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Learn why the same bit pattern can mean different things depending on signedness, and how that difference appears as signed versus unsigned comparisons in a compiled binary.

Day 6 introduced conditional jumps and mentioned that `JG/JL` and `JA/JB` exist for a reason. This lab revisits that idea with a small example.

## Source Code

```c
#include <stdio.h>

int main(void) {
    int signed_value = -1;
    unsigned int unsigned_value = 4294967295U;

    if (signed_value < 100) {
        printf("signed is less than 100\n");
    } else {
        printf("signed is not less than 100\n");
    }

    if (unsigned_value < 100) {
        printf("unsigned is less than 100\n");
    } else {
        printf("unsigned is not less than 100\n");
    }

    printf("%d\n", signed_value);
    printf("%u\n", unsigned_value);

    return 0;
}
```

Compile without optimization first so the two comparisons stay easy to inspect:

```bash
gcc -g -O0 signed_unsigned.c -o signed_unsigned
```

Then import the binary into Ghidra and inspect `main`.

## Same Bits, Different Meaning

On a typical 32-bit `int`, both variables can store this bit pattern:

```text
11111111 11111111 11111111 11111111
```

That pattern is hexadecimal `0xFFFFFFFF`.

As a signed `int`, it is commonly interpreted as `-1`.

As an `unsigned int`, it is `4294967295`.

So the memory contents can look identical while the program logic is different:

```text
signed_value    -> -1
unsigned_value  -> 4294967295
```

This is why reverse engineering is not only about reading bytes. It is also about asking how those bytes are being interpreted.

## Why The Comparisons Diverge

At source level the two `if` statements look almost the same:

```c
if (signed_value < 100)
if (unsigned_value < 100)
```

Their results are not the same:

```text
-1 < 100                 -> true
4294967295 < 100         -> false
```

The compiler has to encode that difference. One comparison treats the value as signed. The other treats it as unsigned.

## Signed Jumps vs Unsigned Jumps

Day 6 showed this split:

```text
JG / JL  -> signed comparisons
JA / JB  -> unsigned comparisons
```

A useful beginner mapping is:

```text
JL  -> jump if less          (signed)
JG  -> jump if greater       (signed)
JB  -> jump if below         (unsigned)
JA  -> jump if above         (unsigned)
```

The words *less* and *greater* belong to signed thinking. The words *below* and *above* belong to unsigned thinking.

You may see a pattern similar to:

```text
CMP  signed_value, 100
JL   ...
```

and later:

```text
CMP  unsigned_value, 100
JB   ...
```

Do not expect the exact same registers or labels on every system. The important clue is the family of jump being used.

## What To Look For In Ghidra

After compiling, inspect `main` and try to locate:

1. The constant `0xFFFFFFFF` or `-1`
2. The two comparisons against `100` / `0x64`
3. Whether one branch uses a signed jump and the other an unsigned jump
4. How Ghidra's decompiler types the two local variables
5. The two `printf` format strings `%d` and `%u`

Ghidra may already reconstruct something close to:

```c
int signed_value;
unsigned int unsigned_value;
```

If the binary were stripped and types were less obvious, the jump family would still be a useful clue:

```text
JL / JG  -> this comparison is probably signed
JB / JA  -> this comparison is probably unsigned
```

## Sign Extension And Zero Extension

When a smaller value is copied into a larger register, the compiler may need to fill the extra bits.

A simplified idea:

```text
MOVSX  -> copy and keep the sign   (signed)
MOVZX  -> copy and fill with zeros (unsigned)
```

You do not need to memorize every extension instruction yet. If you see extra bits being filled from the sign bit, the value is being treated as signed. If the extra bits become zero, it is being treated as unsigned.

## Reverse Engineering Connection

A useful analysis question is:

```text
I see a comparison.
Is this value being treated as signed or unsigned?
```

That question can change the reconstructed logic completely. The same `0xFFFFFFFF` can mean:

```text
"this is -1"
```

or:

```text
"this is a very large number"
```

depending on the comparison that follows.

## Main Takeaway

```text
same bits            -> not always the same meaning
signed int           -> can be negative
unsigned int         -> only zero and above
JL / JG              -> signed comparison
JB / JA              -> unsigned comparison
RE task              -> recover how the value is interpreted
```

The goal is not to memorize every integer conversion rule yet. The goal is to recognize that signedness is part of the program's logic, and that Ghidra's jumps and reconstructed types are clues for recovering it.
