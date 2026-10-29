# Day 42 — Function Pointers and Indirect Calls

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Function Pointers and Indirect Calls** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. A call through a function pointer shows up as `call reg` instead of `call <name>`.
2. The target is only known at run time, so static disassembly cannot name it.
3. Use a debugger breakpoint on the call or inspect the pointer table to resolve targets.

## Important distinctions
- Finding an indicator does not validate every inference about it.
- Identify the trust boundary and the party controlling each input.
- Use both positive and negative cases; document measurement limitations.

## Hands-on lab
Read `indirect.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 indirect.c -o indirect
~~~

~~~bash
./indirect
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn indirect | awk '/<apply>:/,/ret/'
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
