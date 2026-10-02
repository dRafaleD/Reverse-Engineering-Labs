# Day 21 — GDB Basics: Breakpoints, Registers and Stepping Through Execution

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Move from purely static inspection into basic dynamic analysis with GDB.

Until now, many labs asked:

> What does this binary appear to do?

Dynamic analysis adds another question:

> What does the program actually do while it is running?

This lab introduces:

- static vs dynamic analysis
- debug symbols
- breakpoints
- stepping
- stack frames
- register inspection
- local variables
- memory examination
- source, assembly and runtime state
- comparing GDB observations with Ghidra

The program is a harmless local training binary.

## 1. Static vs dynamic analysis

Static analysis examines a program without executing it.

Examples:

- Ghidra decompiler
- `objdump`
- `readelf`
- `strings`

Dynamic analysis observes a program while it executes.

Examples:

- break at a function
- inspect registers
- inspect local variables
- step through instructions
- observe memory at runtime

Neither replaces the other.

A useful workflow is:

```text
static hypothesis
      ↓
choose interesting location
      ↓
run under debugger
      ↓
observe runtime state
      ↓
confirm or revise hypothesis
```

## 2. Training source

The source contains three functions:

```text
main
 ├─ transform
 └─ classify
```

`transform()` performs simple arithmetic.

`classify()` contains a branch.

That gives us function calls, local variables and a conditional path to observe in GDB.

## 3. Compile for debugging

From the lab directory:

```bash
gcc -g -O0 -fno-omit-frame-pointer source/debug_target.c -o debug_target
```

Options:

- `-g` includes debug information.
- `-O0` reduces compiler optimization, making source-to-machine-code relationships easier to follow.
- `-fno-omit-frame-pointer` keeps frame-pointer behavior easier to inspect in this beginner lab.

Run normally first:

```bash
./debug_target
```

Expected output:

```text
input=10 result=23 category=1
```

## 4. Start GDB

```bash
gdb ./debug_target
```

Inside GDB:

```gdb
break main
run
```

The program stops at the breakpoint instead of immediately finishing.

A breakpoint is a controlled pause point.

## 5. Breakpoints by symbol

Because this build contains symbols, you can use function names:

```gdb
break transform
break classify
info breakpoints
```

Then:

```gdb
run
```

When execution reaches one of these functions, GDB pauses.

This demonstrates why symbols make debugging and reverse engineering easier.

Day 20 showed what happens when many helpful symbols are stripped.

## 6. continue, next and step

Three important commands:

```gdb
continue
next
step
```

### continue

Resume execution until another breakpoint, signal or program exit.

### next

Execute the current source line and generally step **over** called functions.

### step

Execute the current source line and step **into** a called function when debug information allows it.

A simple experiment:

1. break at `main`
2. run
3. use `next` until the `transform()` call
4. restart
5. use `step` at the same call

Compare the behavior.

## 7. Inspect source-level variables

At a useful breakpoint:

```gdb
info locals
print input
print result
```

Variables may not yet have meaningful initialized values if execution has not reached their assignments.

Runtime state depends on **where execution is paused**.

This is one of the most important debugger habits:

> Always know where you stopped before interpreting values.

## 8. Inspect arguments

Break at `transform`:

```gdb
break transform
run
info args
print value
```

On the first call, `value` should correspond to the argument passed by `main`.

This connects source-level function parameters to the calling-convention concepts from earlier labs.

## 9. Registers

On x86-64 Linux:

```gdb
info registers
```

You will see registers such as:

- `rax`
- `rbx`
- `rcx`
- `rdx`
- `rsi`
- `rdi`
- `rsp`
- `rbp`
- `rip`

Important beginner roles:

```text
RIP -> current/next instruction location
RSP -> current stack pointer
RBP -> commonly useful as a frame reference in this unoptimized build
RAX -> commonly used for return values
```

Under the System V AMD64 calling convention, integer/pointer arguments commonly begin in registers such as `RDI`, `RSI`, `RDX`, `RCX`, `R8`, and `R9`.

At `transform(int value)`, compare:

```gdb
print value
p/x $rdi
```

Do not assume every optimized binary will preserve such a clean source-level relationship.

## 10. Program counter and current instruction

Inspect the current instruction:

```gdb
x/i $rip
```

Show nearby instructions:

```gdb
disassemble transform
```

or:

```gdb
disassemble /m transform
```

when source intermixing is available.

Now connect:

```text
C source
   ↓
Ghidra decompilation
   ↓
assembly
   ↓
RIP at runtime
```

## 11. Instruction stepping

Source-level stepping is useful, but reverse engineering often needs instruction-level stepping.

Commands:

```gdb
stepi
nexti
```

- `stepi`: execute one machine instruction.
- `nexti`: execute one instruction while stepping over calls when possible.

Use them inside `transform` and watch how `$rip` changes.

```gdb
x/i $rip
stepi
x/i $rip
```

## 12. Stack frames

While stopped inside `transform`:

```gdb
backtrace
frame
info frame
```

A backtrace may conceptually show:

```text
transform
main
```

This is the runtime version of the call-stack ideas from Day 5 and Day 16.

You are no longer only reading a diagram—you are observing active frames.

## 13. Stack memory

Inspect a small region near the stack pointer:

```gdb
x/8gx $rsp
```

This means:

```text
x      -> examine memory
8      -> eight units
g      -> giant words (8 bytes)
x      -> hexadecimal display
```

Do not expect every value to be immediately meaningful.

The stack can contain saved state, return-related information, locals, padding and other runtime data depending on compiler behavior.

## 14. Addresses of local variables

Inside a function with visible locals:

```gdb
print &doubled
print &adjusted
```

Then examine one directly:

```gdb
x/wd &doubled
```

This connects:

```text
C variable
   ↓
runtime address
   ↓
memory bytes
```

The exact address can vary between executions and systems.

## 15. Observe the branch

Break at `classify`:

```gdb
break classify
run
```

Then:

```gdb
print value
disassemble classify
```

Use `stepi` to move toward the comparison and conditional jump.

The C logic:

```c
if (value > 20)
```

will become a comparison plus control-flow decision in machine code.

This connects directly to Day 6.

## 16. Change the source, not the debugger

For this beginner lab, test the other branch by changing:

```c
int input = 10;
```

to a smaller value such as:

```c
int input = 5;
```

Recompile and debug again.

Then compare which path `classify()` takes.

This keeps the exercise focused on observation rather than runtime patching.

## 17. Ghidra + GDB workflow

Open `debug_target` in Ghidra.

Find:

- `main`
- `transform`
- `classify`

Before running GDB, predict:

1. which function receives `10`,
2. what `transform` returns,
3. which branch `classify` takes.

Then verify those predictions dynamically.

This is a core reverse-engineering habit:

```text
read -> predict -> observe -> explain
```

## 18. What changes with optimization?

Compile another build:

```bash
gcc -g -O2 source/debug_target.c -o debug_target_O2
```

Compare it with the `-O0` build.

Optimization may:

- inline functions,
- eliminate variables,
- reorder operations,
- keep values only in registers,
- make source-level stepping look less intuitive.

If GDB reports a value as optimized out, that is not automatically a debugger failure.

It may reflect the fact that the source variable no longer exists as a simple runtime object.

## 19. What changes when symbols are stripped?

Create a copy:

```bash
cp debug_target debug_target_stripped
strip debug_target_stripped
```

Then:

```bash
gdb ./debug_target_stripped
```

Compare function-name visibility with the original.

The program still contains machine code, but many convenient names/debug details are gone.

This directly extends Day 20.

## 20. ASLR awareness

Modern systems commonly randomize parts of process memory layout.

Therefore absolute runtime addresses may differ between executions.

For now:

- prefer symbol-based breakpoints in the debug build,
- focus on relationships rather than memorizing addresses,
- expect addresses to vary.

Later labs can explore process memory layout and ASLR in more depth.

## Safe workflow

```text
compile harmless target
       ↓
inspect statically
       ↓
set breakpoint
       ↓
run under GDB
       ↓
inspect args / locals / registers
       ↓
step through instructions
       ↓
compare with Ghidra
       ↓
write observations
```

## Exercises

1. Compile the `-O0` debug build.
2. Break at `main`.
3. Use both `next` and `step` around the `transform` call.
4. Break at `transform` and inspect `value`.
5. Compare `value` with `$rdi` on x86-64 Linux.
6. Display the current instruction with `x/i $rip`.
7. Use `stepi` for several instructions.
8. Run `backtrace` while inside `transform`.
9. Inspect a small region around `$rsp`.
10. Break at `classify` and observe the branch.
11. Change the source input, rebuild and observe the other path.
12. Compare `-O0`, `-O2` and stripped builds.

## Questions

1. Static analysis vs dynamic analysis?
2. What is a breakpoint?
3. What is the difference between `next` and `step`?
4. What is the difference between `nexti` and `stepi`?
5. What does `RIP` represent?
6. What does `RSP` represent?
7. Why are symbols useful in a debugger?
8. Why might a variable be “optimized out”?
9. Why can runtime addresses change between runs?
10. Why is combining Ghidra and GDB stronger than using only one?

## Main takeaway

```text
static analysis
      ↓
hypothesis
      ↓
breakpoint
      ↓
runtime state
      ↓
registers + memory + control flow
      ↓
verified understanding
```

Day 21 is the transition from only reading compiled programs to watching them execute.
