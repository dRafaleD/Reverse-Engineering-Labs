# Day 48 — Bounds Checks and Signed Versus Unsigned Compares

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Bounds Checks and Signed Versus Unsigned Compares** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. A signed compare uses `jl`/`jg`, an unsigned compare uses `jb`/`ja`; the jump choice reveals the variable's type.
2. A bounds check written as `i < n` on a signed index accepts negative values, which then index memory before the array.
3. Checking `i >= 0 && i < n`, or using an unsigned index, closes the gap.

## Important distinctions
- This lab only reads values safely; it never indexes with a negative number.
- The jump mnemonic is evidence of the type, not proof of the source.
- Static analysis tools flag such compares, but confirm by reading the code.

## Hands-on lab
Read `bounds.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 bounds.c -o bounds
~~~

~~~bash
./bounds
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn bounds | awk '/<weak_get>:/,/ret/'
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn bounds | awk '/<strict_get>:/,/ret/'
~~~


## Assignments
1. Predict the output of both functions for an index of 2.
2. Compile and identify the jump instruction in each function.
3. Describe in words what `weak_get(a, -1, 4)` would read, without running it.
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
