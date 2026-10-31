# Day 47 — Division by Constants and Magic Multiplication

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Division by Constants and Magic Multiplication** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. Optimizing compilers replace division by a constant with a multiplication by a magic number and a shift.
2. For unsigned division by 10 you will typically see a multiplication by `0xCCCCCCCD` followed by a right shift.
3. Recognizing the pattern saves time: a `div` instruction is not used, yet the source says `/ 10`.

## Important distinctions
- At `-O0` the compiler usually emits a real `div`; the magic form appears with optimization.
- The exact constant depends on the divisor and on signedness.
- Verify by testing the function with a few inputs in a debugger.

## Hands-on lab
Read `divconst.c` before running the following from this lab folder:

~~~bash
gcc -g -O2 divconst.c -o divconst
~~~

~~~bash
./divconst
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn divconst | awk '/<div10>:/,/ret/'
~~~


## Assignments
1. Predict the program output.
2. Compile with `-O2` and find the multiplication by the magic constant.
3. Compile with `-O0` and compare with the `div` form.
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
