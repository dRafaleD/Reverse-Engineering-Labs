# Day 37 — Calling Conventions and Argument Tracing in GDB

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Calling Conventions and Argument Tracing in GDB** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. On typical x86-64 System V ABI, the first integer arguments use RDI, RSI and RDX.
2. At -O0, debugger variable inspection is usually more straightforward than with optimization.
3. Debug symbols improve names but do not independently establish source-code integrity.

## Important distinctions
- Finding an indicator does not validate every inference about it.
- Identify the trust boundary and the party controlling each input.
- Use both positive and negative cases; document measurement limitations.

## Hands-on lab
Read `calls.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 -fno-omit-frame-pointer calls.c -o calls
~~~

~~~bash
./calls
~~~

~~~bash
gdb -q -ex 'break calculate' -ex run -ex 'info args' -ex 'bt' -ex quit ./calls
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
