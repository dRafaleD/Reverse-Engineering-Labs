# Day 44 — String Comparison Loops in Disassembly

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **String Comparison Loops in Disassembly** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. A hand-written string comparison usually shows up as a loop that loads one byte from each string and compares them.
2. The loop exits on the first difference or on the terminating zero byte; both exits are conditional jumps.
3. A compiler may replace the loop with a library call such as `strcmp`, so check the call table too.

## Important distinctions
- Byte loads (`movzx`) tell you the comparison is per character, not per word.
- Early exit leaks timing, which matters for secrets; this lab shows it for learning.
- Compare `-O0` and `-O2` output.

## Hands-on lab
Read `streq.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 streq.c -o streq
~~~

~~~bash
./streq
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn streq | awk '/<same>:/,/ret/'
~~~


## Assignments
1. Predict the program output.
2. Compile and find the two conditional jumps that exit the loop.
3. Recompile with `-O2` and describe what changes.
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
