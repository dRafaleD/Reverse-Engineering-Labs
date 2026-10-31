# Day 43 — Struct Return Values: RAX:RDX and Hidden Pointers

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Struct Return Values: RAX:RDX and Hidden Pointers** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. On x86-64 System V, a small struct of two integers (16 bytes) is returned in `rax` and `rdx`.
2. A larger struct is returned through a hidden first argument: the caller passes a pointer in `rdi`, and the callee also returns that pointer in `rax`.
3. In disassembly the caller reserves stack space and passes its address, so an unexplained `lea rdi, [rbp-...]` before a `call` often means a struct return.

## Important distinctions
- The 16-byte boundary and the field types decide the register class; floats use xmm registers.
- Optimization can inline the call and remove the hidden pointer entirely, so compare `-O0` with `-O2`.
- Verify against the ABI document and a debugger, not only against memory.

## Hands-on lab
Read `structret.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 structret.c -o structret
~~~

~~~bash
./structret
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn structret | awk '/<make_small>:/,/ret/'
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn structret | awk '/<make_big>:/,/ret/'
~~~


## Assignments
1. Predict the program output from the source.
2. Compile and run it, then find where `make_small` places its two fields in the disassembly.
3. In `make_big`, find the register that carries the hidden pointer and the store instructions that fill it.
4. Recompile with `-O2` and describe what happens to both calls.
5. Add a third `long` field to the small struct and observe how the calling convention changes.
6. Confirm in GDB with `finish` and `info registers rax rdx`.

## Knowledge check
1. Where does the input originate, and who controls it?
2. What was directly observed, as opposed to inferred?
3. What assumptions might fail in production?
4. Which evidence would be insufficient on its own?
5. What should an automated test check?

## Safety and limitations
Use only authorized systems and synthetic data. The example is not a complete security or detection product.
