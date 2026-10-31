# Day 49 — Tail Calls: jmp Instead of call

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Tail Calls: jmp Instead of call** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. When a function's last action is to return another call's result, an optimizer can replace `call` and `ret` with a single `jmp`.
2. The callee then returns directly to the original caller, so the middle function disappears from the backtrace.
3. A `jmp` to a function name at the end of a function is the recognizable sign.

## Important distinctions
- At `-O0` the same code uses a normal `call`; compare the two builds.
- Missing frames in a backtrace can come from tail calls, not only from corruption.
- Confirm in a debugger by stepping into the call.

## Hands-on lab
Read `tailcall.c` before running the following from this lab folder:

~~~bash
gcc -g -O2 tailcall.c -o tailcall
~~~

~~~bash
./tailcall
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn tailcall | awk '/<outer>:/,/ret|jmp/'
~~~


## Assignments
1. Predict the program output.
2. Compile with `-O2` and find the `jmp` in `outer`.
3. Compile with `-O0` and compare with the `call` form.
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
