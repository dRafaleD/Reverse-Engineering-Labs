# Day 46 — Switch Statements: Jump Tables Versus Compare Chains

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning objective
This day explores **Switch Statements: Jump Tables Versus Compare Chains** with harmless local fixtures. The goal is to reason about controls, evidence, and limits rather than treating one output as proof.

## Core concepts
1. A switch over dense, small values is often compiled to an indirect jump through a table.
2. A switch over sparse values is usually a chain of compares and conditional jumps.
3. A bounds check (`cmp` and `ja`) before the indirect jump is the signature of a table-based switch.

## Important distinctions
- The compiler chooses the form; source style alone does not decide it.
- Tables live in read-only data; follow the jump to see the targets.
- Decompilers may show both forms as a switch.

## Hands-on lab
Read `switch_forms.c` before running the following from this lab folder:

~~~bash
gcc -g -O2 switch_forms.c -o switch_forms
~~~

~~~bash
./switch_forms
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn switch_forms | awk '/<dense>:/,/ret/'
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn switch_forms | awk '/<sparse>:/,/ret/'
~~~


## Assignments
1. Predict which function uses a jump table.
2. Compile and find the indirect `jmp` in the dense function.
3. Make the values denser or sparser and see when the form changes.
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
