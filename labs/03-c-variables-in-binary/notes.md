# Day 3 — C Variables Inside a Binary

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

The goal of this lab is to connect basic C variable types with the way they appear inside a compiled binary.

We are not trying to memorize assembly yet. The important part is to recognize simple patterns in Ghidra.

## Source Code

```c
#include <stdio.h>

int main() {
    char mychar = 'C';
    int myinteger = 16;
    float myfloat = 5.10f;
    double mydouble = 213.568;

    printf("%c\n", mychar);
    printf("%d\n", myinteger);
    printf("%f\n", myfloat);
    printf("%f\n", mydouble);

    return 0;
}
```

Compile with debug information:

```bash
gcc -g variables.c -o variables
```

Then import the executable into Ghidra and open the `main` function.

## Variable Sizes

On the system used for this lab, the basic sizes were:

```text
char   -> 1 byte
int    -> 4 bytes
float  -> 4 bytes
double -> 8 bytes
```

These sizes help us understand why Ghidra may show different pointer sizes for different variables.

## What Ghidra Showed

### `char`

A simplified line looked like:

```asm
MOV byte ptr [RBP + mychar], 0x43
```

- `byte ptr` means 1 byte.
- `0x43` is hexadecimal.
- `0x43` corresponds to the ASCII character `C`.

This connects directly to:

```c
char mychar = 'C';
```

### `int`

A simplified line looked like:

```asm
MOV dword ptr [RBP + myinteger], 0x10
```

- `dword ptr` means 4 bytes.
- `0x10` in hexadecimal is `16` in decimal.

This connects to:

```c
int myinteger = 16;
```

### `float`

Ghidra used instructions such as:

```asm
MOVSS
```

`MOVSS` is commonly used when moving a single-precision floating-point value.

For this lab, the simple connection is:

```text
MOVSS -> float
```

### `double`

Ghidra used instructions such as:

```asm
MOVSD
```

For this lab, the simple connection is:

```text
MOVSD -> double
```

## Pointer Size Keywords

The following words appeared in Ghidra:

```text
byte ptr  -> 1 byte
dword ptr -> 4 bytes
qword ptr -> 8 bytes
```

This matches the variable sizes seen earlier:

```text
char   -> byte ptr
int    -> dword ptr
float  -> dword ptr
double -> qword ptr
```

This is one of the first useful links between C data types and assembly-level memory access.

## Registers Seen

### `RBP`

When Ghidra shows something like:

```text
[RBP + myinteger]
```

it is accessing a local variable inside the current function's stack frame.

For now, it is enough to think of `RBP` as a reference point used to reach local stack variables.

### `XMM0` and `XMM1`

These registers appeared around `float` and `double` operations.

They are commonly used for floating-point and SIMD operations on x86-64.

They do not need to be memorized yet.

## `CALL printf`

The binary also contained instructions such as:

```asm
CALL printf
```

`CALL` means that another function is being called.

This connects directly to the `printf()` calls in the C source.

## Main Takeaway

The important idea from Day 3 is this:

```text
C variable
    ↓
size in bytes
    ↓
memory access size in assembly
    ↓
Ghidra representation
```

Examples:

```text
char   -> 1 byte -> byte ptr
int    -> 4 byte -> dword ptr
float  -> 4 byte -> MOVSS / dword ptr
double -> 8 byte -> MOVSD / qword ptr
```

The goal is not to memorize every instruction. The goal is to start recognizing how simple C concepts leave traces inside the compiled binary.
