# Day 13 — Function Pointers and Indirect Calls

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Learn what a function pointer is and why it may appear as an indirect call in a compiled binary.

## Source code

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int main() {
    int (*operation)(int, int) = add;

    int result1 = operation(10, 4);
    printf("Add result: %d\n", result1);

    operation = subtract;

    int result2 = operation(10, 4);
    printf("Subtract result: %d\n", result2);

    return 0;
}
```

## What is a function pointer?

A normal pointer stores an address. A function pointer stores the address of a function.

```c
int (*operation)(int, int) = add;
```

This means:

```text
operation
   ↓
address of add()
```

Later, the same pointer can be changed:

```c
operation = subtract;
```

Now it points to `subtract()` instead.

## Direct call vs indirect call

A direct function call has a known destination:

```text
CALL add
```

With a function pointer, the destination comes from an address stored in a register or memory location. In assembly this may appear conceptually like:

```text
CALL RAX
```

or:

```text
CALL qword ptr [address]
```

This is called an **indirect call**.

The exact register and instruction sequence can change depending on the compiler and optimization settings.

## Why indirect calls matter in reverse engineering

Direct calls are easy to follow because the target function is visible immediately.

Indirect calls require one extra question:

```text
Where did this function address come from?
```

A useful analysis flow is:

```text
indirect CALL
    ↓
find the register/pointer used by CALL
    ↓
trace where its value was assigned
    ↓
identify possible target function(s)
```

This pattern appears in normal software in places such as callbacks, dispatch tables, event handlers, plugin systems, and object-oriented code.

## What to inspect in Ghidra

Compile without optimization first so the relationship is easier to observe:

```bash
gcc -g -O0 function_pointers.c -o function_pointers
```

Then inspect `main`, `add`, and `subtract`.

Look for:

- the addresses of `add` and `subtract`
- where the function pointer is stored
- where it is loaded again
- a `CALL` whose destination comes through a register or pointer

You may see a pattern similar to:

```text
LEA   RAX, [add]
MOV   [local_pointer], RAX
...
MOV   RDX, [local_pointer]
...
CALL  RDX
```

Do not expect the exact same registers on every system.

## Arguments still follow the calling convention

Even when the target is indirect, function arguments are still passed according to the platform's calling convention.

For example, on Linux x86-64 System V, integer arguments commonly begin in:

```text
RDI -> first argument
RSI -> second argument
```

So before an indirect call you may still find the values `10` and `4` being prepared in argument registers.

## Reverse engineering connection

At source level:

```c
operation(10, 4);
```

At binary level, think more like:

```text
prepare arguments
      ↓
load function address
      ↓
CALL through that address
      ↓
receive return value
```

The important difference is that the `CALL` instruction may not directly name `add` or `subtract`.

## Main takeaway

```text
function pointer -> stores a function address
direct CALL      -> target is encoded directly
indirect CALL    -> target comes from a register/memory
RE task          -> trace where that target address came from
```

The goal is not to memorize every possible indirect-call pattern yet. The goal is to recognize that a `CALL` can use a dynamically selected destination instead of a fixed function name.
