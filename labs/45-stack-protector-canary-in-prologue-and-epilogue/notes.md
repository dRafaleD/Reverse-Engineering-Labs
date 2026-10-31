# Day 45 — Stack Protector Canary in Prologue and Epilogue

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Stack Protector Canary in Prologue and Epilogue** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. With `-fstack-protector-strong`, a function with a local buffer reads a canary from `fs:0x28` in its prologue and stores it on the stack.
2. Before returning, the epilogue compares the stored value with the original and calls `__stack_chk_fail` if they differ.
3. Seeing this pattern tells you the binary was built with stack protection, which also helps to find the local buffer.

## Important distinctions
- The canary detects some overwrites at return time; it does not prevent all memory bugs.
- Whether a function gets a canary depends on compiler flags and its locals.
- This lab only reads the code; it does not trigger any overflow.

## Hands-on lab
Read `canary.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 -fstack-protector-strong canary.c -o canary
~~~

~~~bash
./canary
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn canary | grep -E 'fs:0x28|__stack_chk_fail'
~~~


## Assignments
1. Predict whether `-fno-stack-protector` shows the canary instructions.
2. Compile with `-fstack-protector-strong` and find the `fs:0x28` accesses.
3. Compile again without the protector and compare the two disassemblies.
4. Add an assertion for one security property.
5. Describe a negative case and a potential false positive.
6. Propose an independent corroboration method.

## Knowledge check
1. Where does the input originate, and who controls it?
2. What was directly observed, as opposed to inferred?
3. What assumptions might fail in production?
4. Which evidence would be insufficient on its own?
5. What should an automated test check?

## Safety and limitations
Use only authorized systems and synthetic data. The example is not a complete security or detection product.
