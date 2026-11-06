# Day 51 — String Literals and the .rodata Section

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **String Literals and the .rodata Section** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. String literals are stored in a read-only data section, usually `.rodata`, and code refers to them by address.
2. In disassembly a `lea` of an address inside `.rodata` followed by a call to `puts` or `printf` is the typical way a literal is used.
3. Dumping the section shows every literal, which is a quick way to find interesting code to analyze.

## Important distinctions
- Literals are shared and deduplicated, so addresses can repeat.
- A stripped binary still keeps its literals, which is why strings remain useful.
- Writing to a literal at runtime is a bug and typically crashes.

## Hands-on lab
Read `rodata.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 rodata.c -o rodata
~~~

~~~bash
./rodata
~~~

~~~bash
objdump -s -j .rodata rodata
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn rodata | awk '/<main>:/,/ret/'
~~~


## Assignments
1. Predict the program output.
2. Dump `.rodata` and find the two literals.
3. Find the `lea` in `main` that loads one of the literals.
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
