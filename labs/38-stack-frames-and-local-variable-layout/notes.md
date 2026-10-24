# Day 38 — Stack Frames and Local Variable Layout in GDB

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Stack Frames and Local Variable Layout in GDB** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. Each call creates a frame holding the saved return address, the saved frame pointer and locals.
2. Locals are uninitialized at function entry; values seen before assignment are not meaningful.
3. Frame layout depends on compiler, flags and optimization level.

## Important distinctions
- Finding an indicator does not validate every inference about it.
- Identify the trust boundary and the party controlling each input.
- Use both positive and negative cases; document measurement limitations.

## Hands-on lab
Read `frames.c` before running the following from this lab folder:

~~~bash
gcc -g -O0 -fno-omit-frame-pointer frames.c -o frames
~~~

~~~bash
./frames
~~~

~~~bash
gdb -q -batch -ex 'break sum_locals' -ex run -ex 'info frame' -ex 'info locals' -ex finish ./frames
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
