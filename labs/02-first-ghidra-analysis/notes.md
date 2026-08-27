[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

# Day 2 — First Ghidra Analysis

## Goal

The goal of this lab is to inspect the binary produced from the simple Day 1 C program and understand how a few source-level concepts appear inside Ghidra.

The source program used for this analysis is the Day 1 `hello.c` file:

```c
#include <stdio.h>

int main() {
    printf("This is my first c code\n");
    return 0;
}
```

Compile it with debug information:

```bash
gcc -g hello.c -o hello
```

Then import the resulting ELF executable into Ghidra and analyze the `main` function.

## The `main` Function in Ghidra

A simplified version of the relevant assembly looked like this:

```asm
PUSH RBP
MOV  RBP,RSP
LEA  RAX,["This is my first c code"]
MOV  RDI,RAX
CALL puts
MOV  EAX,0x0
POP  RBP
RET
```

## What I Learned

### Function Header

Ghidra identifies the function as:

```text
int main(void)
```

The return value is associated with the `EAX` register on x86-64.

### `PUSH RBP`

Saves the previous value of the `RBP` register on the stack.

For now, this can be understood as part of the function setup process.

### `MOV RBP, RSP`

Copies the current stack pointer (`RSP`) into `RBP`.

Together with `PUSH RBP`, this is part of setting up the stack frame for the function.

### `LEA RAX, [...]`

`LEA` means **Load Effective Address**.

In this program, Ghidra shows it loading the address of the string:

```text
"This is my first c code"
```

into the `RAX` register.

The important idea is that the string itself is stored somewhere in memory, and the program works with its address.

### `MOV RDI, RAX`

Copies the address stored in `RAX` into `RDI`.

On Linux x86-64 using the System V calling convention, `RDI` is normally used for the first function argument.

This means the address of the string is being prepared as the first argument for the next function call.

### `CALL puts`

Calls the external `puts()` function.

An interesting observation is that the original C source used:

```c
printf("This is my first c code\n");
```

but the compiled binary calls `puts()` instead.

This is an important reverse engineering lesson:

> Compiled machine code does not always look exactly like the original source code.

The compiler can transform or optimize source-level operations into equivalent instructions or function calls.

### `MOV EAX, 0x0`

Places `0` into the `EAX` register.

This corresponds to:

```c
return 0;
```

from the source program.

### `POP RBP`

Restores the previous value of `RBP` from the stack.

This is part of cleaning up the function before returning.

### `RET`

Returns execution to the caller of the current function.

In simple terms, this marks the end of `main`.

## Registers Seen in This Lab

- `RAX` — general-purpose register; also related to function return values (`EAX` is its lower 32-bit part)
- `RDI` — normally holds the first function argument on Linux x86-64 System V
- `RSP` — stack pointer
- `RBP` — commonly used as a stack-frame/base reference
- `EAX` — lower 32 bits of `RAX`; used here for the integer return value of `main`

These do not need to be memorized yet. The goal is only to begin recognizing them when they appear again.

## Ghidra Concepts

### Instruction

An instruction is a single command executed by the CPU.

Examples from this lab:

```asm
MOV
LEA
CALL
PUSH
POP
RET
```

### Address

Ghidra shows a virtual memory address beside each instruction, for example:

```text
00101159
```

This represents where Ghidra maps that instruction in the program's memory view.

### Machine-Code Bytes

Ghidra also shows the raw bytes that encode each instruction.

For example, an assembly instruction may appear next to bytes such as:

```text
48 89 e5
```

The CPU executes these encoded bytes; Ghidra translates them into human-readable assembly.

### XREF

`XREF` means **Cross Reference**.

It shows other locations in the binary that reference a function, address, or piece of data.

## ELF and `.fini`

The compiled Linux executable uses the ELF (**Executable and Linkable Format**) file format.

An ELF file contains more than the instructions written directly by the programmer. It also contains metadata, sections, runtime-related code, imported functions, constants, and other information required to load and execute the program.

Ghidra may therefore show functions and sections that were not directly written in the C source.

One example is:

```text
.fini
```

The `.fini` section is related to finalization/cleanup code used by the ELF runtime/toolchain. For these early labs, it is enough to recognize that it is not part of the main program logic written in `main()`.

## File Offset vs Virtual Address

A **file offset** describes where bytes are located inside the executable file on disk.

A **virtual address** describes where code or data appears in the program's memory address space after it is mapped for execution.

They answer two different questions:

```text
File offset     -> Where is it inside the file?
Virtual address -> Where is it in the program's memory view?
```

## Simplified Program Flow

The `main` function can be understood approximately as:

```text
main starts
    ↓
prepare stack frame
    ↓
find the address of the string
    ↓
place the string address in the first argument register
    ↓
call puts
    ↓
set return value to 0
    ↓
restore stack-frame state
    ↓
return
```

## Main Takeaway

The most important lesson from Day 2 is not memorizing assembly instructions.

It is understanding the relationship:

```text
C source code
    ↓
Compiler
    ↓
ELF executable
    ↓
Machine-code bytes
    ↓
Ghidra disassembly
```

The binary is the compiler's result, not a perfect line-by-line copy of the original C source.
