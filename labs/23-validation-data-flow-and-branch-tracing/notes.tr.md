# Gün 23 — Validation Logic, Data Flow ve Ghidra + GDB ile Branch Tracing

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Combine static and dynamic analysis to understand a small validation routine.

This day intentionally combines multiple related skills:

1. locating interesting strings,
2. following XREFs,
3. tracing user input,
4. understanding a helper function,
5. following return values,
6. reading conditional branches,
7. using GDB to verify static hypotheses,
8. comparing source, decompiler, assembly and runtime state.

The objective is not to “crack” software. It is to practice reasoning about control flow and data flow in a harmless binary you compiled yourself.

## 1. Build the training target

Compile:

```bash
gcc -g -O0 -fno-omit-frame-pointer source/validation_flow.c -o validation_flow
```

Run it:

```bash
./validation_flow
```

Try ordinary inputs and observe accepted/rejected behavior.

## 2. Start from strings

In Ghidra, useful strings include:

```text
Enter 4-byte training code:
Accepted
Rejected
```

Follow XREFs from `Accepted` and `Rejected`.

This often leads directly to the decision-making function.

A practical RE workflow is:

```text
interesting string
      ↓
XREF
      ↓
branching code
      ↓
upstream condition
```

## 3. Locate validate()

The source contains:

```c
int validate(const char *input)
```

In a stripped binary this name may be missing, but in this debug build it helps you learn the pattern.

Inside Ghidra, identify:

- the input pointer,
- the call to `checksum`,
- character comparisons,
- the checksum comparison,
- the return values 0 and 1.

## 4. Data flow

The interesting question is:

> Where does the value being compared come from?

Trace:

```text
scanf
  ↓
input buffer
  ↓
validate(input)
  ↓
checksum(input, 4)
  ↓
return value
  ↓
comparison
  ↓
final branch
```

This is data-flow reasoning.

Control flow asks “where execution goes.”

Data flow asks “where values come from.”

Good reverse engineering uses both.

## 5. Helper functions

The program separates logic into:

```text
validate()
checksum()
```

A common beginner mistake is to analyze only the current function and ignore helper calls.

Instead ask:

- What arguments are passed?
- What does the helper return?
- How is that return value used?
- Does the caller branch on it?

## 6. Return value tracking

On x86-64 Linux, integer return values commonly come back in `EAX/RAX`.

A conceptual pattern:

```asm
call checksum
cmp eax, 0x26
jne rejected
```

Exact output may differ.

The important pattern is:

```text
CALL
  ↓
return value
  ↓
CMP / TEST
  ↓
conditional jump
```

## 7. Character comparisons

The validation routine also checks individual bytes.

Conceptually:

```text
input[0] == 'R'
input[1] == 'E'
input[2] == 'V'
input[3] == '!'
```

In assembly, characters often appear as immediate byte values.

Examples:

```text
'R' = 0x52
'E' = 0x45
'V' = 0x56
'!' = 0x21
```

Recognizing ASCII values is useful when string logic is compiled into byte comparisons.

## 8. Branch tracing

For each comparison, determine:

- what is compared,
- what makes the branch continue,
- what makes it fail,
- where the failure path goes.

Draw it as a flow:

```text
check byte 0
   ↓ pass
check byte 1
   ↓ pass
check byte 2
   ↓ pass
check byte 3
   ↓ pass
check checksum
   ↓ pass
return 1
```

Any failed check goes to return 0.

## 9. GDB verification

Start:

```bash
gdb ./validation_flow
```

Break at:

```gdb
break validate
break checksum
run
```

At `validate`:

```gdb
info args
x/s input
```

At `checksum`:

```gdb
info args
x/4bx buf
```

After returning:

```gdb
finish
print $eax
```

Use runtime state to verify your Ghidra interpretation.

## 10. Source vs decompiler

Compare the original source with Ghidra's decompiler.

Look for transformations such as:

- combined conditions,
- temporary variables,
- pointer casts,
- explicit byte comparisons,
- reordered-looking expressions.

Decompiler output is reconstructed pseudocode, not the original source.

## 11. Assembly vs decompiler

When something in the decompiler looks unclear:

1. click the relevant line,
2. inspect the corresponding assembly,
3. identify `CMP`, `TEST`, `JZ`, `JNZ`, `JE`, `JNE`, etc.,
4. map it back to the higher-level logic.

This is an important habit:

```text
decompiler for speed
assembly for ground truth
```

## 12. Stripped comparison

Create:

```bash
cp validation_flow validation_flow_stripped
strip validation_flow_stripped
```

Import both into Ghidra.

Compare:

- function names,
- helper identification,
- strings,
- XREFs,
- decompiler readability.

Try to rediscover the validation routine from the `Accepted`/`Rejected` strings.

## 13. Optimization comparison

Build:

```bash
gcc -g -O2 source/validation_flow.c -o validation_flow_O2
```

Compare with `-O0`.

Optimization may:

- inline helper logic,
- merge branches,
- simplify arithmetic,
- reorder comparisons,
- remove obvious source structure.

This teaches why pattern recognition matters more than expecting source-like output.

## 14. Mini challenge — recover logic without source

Temporarily hide the C source from yourself.

Using only Ghidra:

1. find the accepted/rejected strings,
2. follow XREFs,
3. identify the decision function,
4. list all required conditions,
5. identify the helper function,
6. describe what it computes,
7. write pseudocode from your analysis.

Then compare with the source.

## 15. Mini challenge — runtime proof

Without relying on source:

1. choose one input,
2. break at `validate`,
3. inspect the bytes,
4. step to the checksum call,
5. inspect the return value,
6. step through the final decision,
7. explain exactly why the program accepted or rejected.

## 16. Why this matters

Real binaries often hide meaning behind:

- helper functions,
- indirect data dependencies,
- multiple checks,
- shared failure paths,
- compiler transformations.

Learning to follow values and branches is more important than memorizing individual instructions.

## Exercises

1. Compile the `-O0` target.
2. Find the accepted/rejected strings.
3. Follow XREFs to the decision code.
4. Identify the input buffer.
5. Find `validate` and `checksum`.
6. Trace the checksum return value.
7. Convert the character constants to ASCII.
8. Draw the branch flow.
9. Verify with GDB.
10. Strip the binary and rediscover the logic.
11. Build `-O2` and compare.
12. Complete the source-hidden mini challenge.

## Questions

1. Control flow vs data flow?
2. Why are strings useful starting points?
3. What is an XREF?
4. Why should helper functions be analyzed?
5. Where do integer return values commonly appear on x86-64 Linux?
6. Why can ASCII constants appear as hex values?
7. Why is decompiler output not source code?
8. When should you inspect assembly directly?
9. How does stripping change analysis?
10. How can optimization change branch structure?
11. Why is runtime verification valuable?
12. What is the value of reconstructing pseudocode yourself?

## Main takeaway

```text
strings
   ↓
XREFs
   ↓
input data flow
   ↓
helper calls
   ↓
return values
   ↓
conditional branches
   ↓
GDB verification
   ↓
reconstructed behavior
```

Day 23 is about moving from “I can read instructions” to “I can explain how data drives a decision.”


> Türkçe çalışma notu: Bu günün ana odağı **control flow + data flow birlikte düşünmek**. Sadece “hangi jump nereye gidiyor?” değil, “bu comparison'daki değer nereden geldi?” sorusunu sürekli sor.
