# Day 39 — Conditionals and Branches in Disassembly

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Conditionals and Branches in Disassembly** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. An if statement usually becomes a cmp followed by a conditional jump such as jns or jne.
2. The jump often tests the inverse of the source condition: it skips the body when the condition is false.
3. Disassembly is compiler output; the same source can produce different branches at other optimization levels.

## Important distinctions
- Finding an indicator does not validate every inference about it.
- Identify the trust boundary and the party controlling each input.
- Use both positive and negative cases; document measurement limitations.

## Hands-on lab
Read `branches.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 branches.c -o branches
~~~

~~~bash
./branches
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn branches | awk '/<classify>:/,/ret/'
~~~

## Assignments
1. Predict the output from the code.
2. Run the program and record inputs and outputs.
3. Change a harmless input and explain the resulting difference.
4. Add an assertion for one security property.
5. Describe a negative case and a potential false positive.
6. Propose an independent corroboration method.

## Knowledge check
1. Where does the input originate?
2. What was directly observed?
3. What assumptions might fail in production?
4. Which evidence would be insufficient?
5. What should an automated test check?

## Safety and limitations
Use only authorized systems and synthetic data. The example is not a complete security or detection product.
