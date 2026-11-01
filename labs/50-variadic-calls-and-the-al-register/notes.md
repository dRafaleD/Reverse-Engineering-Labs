# Day 50 — Variadic Calls and the AL Register

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Variadic Calls and the AL Register** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. On x86-64 System V, a variadic call such as printf sets AL to the number of vector registers used.
2. That is why `mov eax, 0` appears right before `call printf`.
3. It is an ABI detail, not a value the program computes from your data.

## Important distinctions
- Finding an indicator does not validate every inference about it.
- Identify the trust boundary and the party controlling each input.
- Use both positive and negative cases; document measurement limitations.

## Hands-on lab
Read `variadic.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 variadic.c -o variadic
~~~

~~~bash
./variadic
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn variadic | awk '/<show>:/,/ret/'
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
