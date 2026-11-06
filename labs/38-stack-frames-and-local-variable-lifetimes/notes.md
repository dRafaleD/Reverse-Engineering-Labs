# Day 38 — Stack Frames, Local Variables and Compiler Optimization

[English](notes.md) · [Türkçe](notes.tr.md)

## Goal
Inspect a harmless C program in GDB to understand stack frames, local variable lifetimes, and why optimized builds change debugging evidence. A stack snapshot is evidence of *one moment*, not a full execution history.

## Fundamentals
- A function call creates an activation record (stack frame); exact layout depends on ABI, compiler and optimization.
- Automatic local variables are normally valid only during their enclosing lifetime. Returning a pointer to a local variable is invalid; do not attempt to dereference it.
- A saved frame pointer is not guaranteed. The compiler can omit it or inline calls.
- Debug symbols help interpret observations but do not prove the binary matches trusted source.

## Exercise
Read `frames.c` first. On Linux with GCC and GDB:

```bash
gcc -std=c11 -Wall -Wextra -g -O0 -fno-omit-frame-pointer frames.c -o frames_debug
./frames_debug
gdb -q -ex 'break inspect' -ex run -ex 'info args' -ex 'info locals' -ex 'bt' -ex continue -ex quit ./frames_debug
gcc -std=c11 -Wall -Wextra -g -O2 frames.c -o frames_opt
./frames_opt
```

Compare visibility of locals between builds; use `disassemble inspect` when needed. Neither address equality nor a particular stack layout is portable.

## Investigation questions
1. Which values are function parameters, and which are local variables?
2. What changes after the function returns? What *cannot* be inferred?
3. How do `-O0` and `-O2` alter debugger observations?
4. How would you distinguish inlining from a missing symbol?

## Evidence checklist
Record compiler version, flags, architecture, debugger commands, and actual observed output. Try both positive and negative cases; never infer maliciousness from one unusual stack trace.

## Safety
Local toy code only. No unsafe reads, exploitation, or third-party binaries.
