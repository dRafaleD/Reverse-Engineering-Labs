# Day 16 — Recursion and Nested Stack Frames

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Learn how a function that calls itself appears in a compiled binary, and why each recursive call creates another stack frame instead of jumping backward like a loop.

This lab connects Day 4 (function calls), Day 5 (stack frames), Day 6 (conditional jumps), and Day 7 (loops).

## Source Code

```c
#include <stdio.h>

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

int main(void) {
    int result = factorial(4);
    printf("%d\n", result);
    return 0;
}
```

Compile without optimization so the recursive call stays easy to inspect:

```bash
gcc -g -O0 recursion.c -o recursion
```

Then import the binary into Ghidra and inspect `factorial`.

## What Is Recursion?

A recursive function solves a smaller version of the same problem by calling itself.

In this example, `factorial(4)` means:

```text
4 * factorial(3)
      3 * factorial(2)
            2 * factorial(1)
                  1
```

The final result is `24`.

The important reverse-engineering idea is not the math. It is that the same function body runs more than once, with a different `n` each time, before the earlier calls finish.

## Recursion Is Not A Loop

Day 7 showed repetition with a backward jump inside one function:

```text
compare
  ↓
jump backward
  ↓
same stack frame runs again
```

Recursion repeats work in a different way:

```text
CALL the same function
  ↓
a new stack frame is created
  ↓
the previous call waits
```

A loop reuses the current frame. A recursive call stacks a new frame on top of the old one.

## The Base Case

Without a stopping condition, recursion would never return.

```c
if (n <= 1) {
    return 1;
}
```

In the binary this is still the Day 6 pattern:

```text
CMP n, 1
  ↓
conditional jump
  ↓
either return 1
or continue into the recursive call
```

When reversing an unknown function, a comparison near the start followed by an early `RET` is often the base case.

## Nested Stack Frames

Each call to `factorial` can have its own copy of `n`.

A simplified picture of `factorial(4)`:

```text
factorial n=4
  factorial n=3
    factorial n=2
      factorial n=1   <- base case returns
    2 * 1
  3 * 2
4 * 6
```

Day 5 showed one function preparing one stack frame. Recursion shows several frames of the same function alive at the same time.

That is why Ghidra may show `factorial` calling `factorial`. The destination is not a different helper. It is another activation of the same code.

## What To Look For In Ghidra

After compiling, open `factorial` and try to locate:

1. The comparison that implements `n <= 1`
2. An early return of `1`
3. Preparation of the argument `n - 1`
4. A `CALL` whose target is `factorial` itself
5. A multiply after the recursive call returns
6. The final `RET`

You may see a pattern similar to:

```text
CMP   n, 1
JLE   base_case
MOV   EDI, n-1
CALL  factorial
IMUL  EAX, n
RET
```

The exact registers can change. The useful shape is:

```text
check base case
  ↓
CALL same function
  ↓
combine the returned value with the current n
```

In `main`, the first call is ordinary: arguments are prepared, then `CALL factorial`. The unusual part is inside `factorial`.

## Why This Matters In Reverse Engineering

If you only look at the listing, recursion can look like a normal function call.

The extra question is:

```text
Does this CALL go to a different function,
or back into the function I am already reading?
```

If the target is the same function:

```text
this may be recursion
  ↓
look for a base case
  ↓
expect nested stack frames
  ↓
later calls return into earlier ones
```

That is different from Day 13, where an indirect call selected among several possible targets. Here the target is fixed, but the function is calling itself.

## Main Takeaway

```text
recursion            -> a function calls itself
base case            -> comparison plus early return
CALL same function   -> new stack frame, not a backward jump
loop                 -> repeats inside one frame
recursion            -> stacks frames until the base case
RE task              -> find the stop condition and how results combine
```

The goal is not to trace every nested frame by hand. The goal is to recognize a self-call, a base case, and the work that happens after the recursive `CALL` returns.
