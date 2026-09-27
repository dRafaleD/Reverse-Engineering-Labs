# Day 19 — Function Pointers Revisited: Callbacks and Indirect Control Flow

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Build on Day 13 by looking at a realistic reason function pointers exist: **callbacks**. The reverse-engineering focus is indirect control flow—situations where a call target is stored in a register or memory instead of appearing as a fixed function address.

## Source

`source/callbacks.c` defines two operations and passes one of them to `calculate`.

Compile a beginner-friendly build:

```bash
gcc -g -O0 source/callbacks.c -o callbacks
```

Then compare with an optimized build:

```bash
gcc -g -O2 source/callbacks.c -o callbacks_O2
```

Optimization can substantially change what you see.

## 1. Direct calls

A normal function call has a known target:

```c
add(6, 7);
```

Conceptually, assembly can contain:

```asm
call add
```

The destination is visible directly in the instruction.

## 2. Indirect calls

Inside `calculate`:

```c
return operation(x, y);
```

`operation` is a function pointer. The program receives the address of a function and calls through that value.

A common assembly idea is:

```asm
call rax
```

or a call through memory.

The important pattern is not the exact register:

```text
function address stored somewhere
        ↓
value reaches call site
        ↓
indirect CALL
        ↓
control transfers to selected function
```

## 3. Callback mental model

A callback means one function receives another function to invoke later.

```text
main
  ↓
choose add
  ↓
calculate(add, 6, 7)
  ↓
operation points to add
  ↓
indirect call
  ↓
add returns 13
```

This pattern appears in event handlers, sorting APIs, plugin systems and many larger programs.

## 4. Why this matters in reverse engineering

Direct calls are easy to follow because the destination is explicit.

Indirect calls require more reasoning:

- Where did the pointer come from?
- Which functions can it point to?
- Was it loaded from a local variable, global table, object, or structure?
- Can multiple targets reach the same call site?

This is the beginning of **control-flow recovery**.

## 5. Ghidra exercise

Open the `-O0` binary first.

Find:

- `main`
- `calculate`
- `add`
- `multiply`

Inside `calculate`, identify the function-pointer parameter and the indirect call.

Then trace backward:

```text
indirect call
    ↑
function-pointer variable
    ↑
argument passed to calculate
    ↑
add or multiply
```

Rename variables if Ghidra gives generic names.

## 6. Function addresses

In `main`, the compiler must somehow pass the selected function's address.

Depending on the binary, you may see address-loading patterns involving `LEA`, registers, stack variables, or relocations.

Do not memorize one instruction sequence. Ask:

> Which value eventually becomes the target of the indirect call?

## 7. Compare -O0 and -O2

Optimization may:

- inline functions,
- remove branches whose result is known,
- propagate constants,
- eliminate the callback path entirely,
- reorganize registers and stack variables.

Our source sets:

```c
int choice = 1;
```

At `-O2`, the compiler may determine the result without preserving the source-level structure you expected.

This is an important reverse-engineering lesson: **source structure and binary structure are not guaranteed to match one-to-one.**

## 8. Static analysis clue

When you see:

```asm
call rax
```

you cannot conclude what function is called from that instruction alone.

You need data-flow context.

A useful workflow:

```text
find indirect call
      ↓
identify target register/memory
      ↓
trace where target value came from
      ↓
collect possible destinations
      ↓
interpret behavior
```

## 9. Security connection

Indirect calls are common in legitimate software. Their presence is not suspicious by itself.

For malware analysis and vulnerability research, however, understanding indirect control flow is important because behavior may be hidden behind callbacks, dispatch tables, virtual functions, imported function pointers, or dynamically resolved APIs.

## Exercises

1. Compile with `-O0` and find the indirect call in `calculate`.
2. Identify which argument carries the callback address.
3. Trace the callback back to `add`.
4. Change `choice` to `0` and compare the behavior.
5. Compile with `-O2` and compare the decompiler output.
6. Write down which source-level functions still exist as separate functions in the optimized binary.

## Questions

1. What is the difference between a direct and indirect call?
2. What is a callback?
3. Why is `call rax` harder to analyze than `call add`?
4. What information do you need to recover an indirect call target?
5. Why can `-O2` make the binary look very different from the source?
6. Does an indirect call imply malicious behavior?

## Main takeaway

Day 13 introduced function pointers. Day 19 uses them as a control-flow problem:

```text
pointer value
    ↓
data flow
    ↓
indirect call
    ↓
possible targets
    ↓
program behavior
```

When an indirect call appears, stop reading only instruction-by-instruction and start tracing **where the target value came from**.
