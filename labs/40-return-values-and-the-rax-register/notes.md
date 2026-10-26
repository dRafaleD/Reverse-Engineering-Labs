# Day 40 — Return Values and the RAX Register

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Return Values and the RAX Register** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. On x86-64 System V, integer and pointer return values travel in `rax`; the 32-bit part `eax` carries an `int`.
2. A caller that uses a result reads `eax`/`rax` right after the `call`; a caller that ignores it does not.
3. A function with no `return` on some path leaves whatever was in `rax`, which is a bug worth recognizing.

## Important distinctions
- Writing to `eax` zeroes the upper half of `rax`; writing to `ax` or `al` does not.
- Optimization may fold the call away entirely, so compare `-O0` with `-O2`.
- Disassembly shows what the compiler emitted, not the programmer's intent.

## Hands-on lab
Read `returns.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 returns.c -o returns
~~~

~~~bash
./returns
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn returns | awk '/<add_one>:/,/ret/'
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn returns | awk '/<main>:/,/ret/'
~~~


## Assignments
1. Predict the program output from the source.
2. Compile and run it, then find where `add_one` places its result in the disassembly.
3. In `main`, find the instruction that reads the result right after the `call`.
4. Recompile with `-O2` and describe what happens to the call.
5. Change the return type to `long` and note whether `eax` becomes `rax`.
6. Verify your reading in GDB with `finish` and `info registers rax`.

## Knowledge check
1. Where does the input originate, and who controls it?
2. What was directly observed, as opposed to inferred?
3. What assumptions might fail in production?
4. Which evidence would be insufficient on its own?
5. What should an automated test check?

## Safety and limitations
Use only authorized systems and synthetic data. The example is not a complete security or detection product.
