# Day 18 — Library Calls, PLT/GOT and String Comparison

## Goal
Connect familiar C library calls to what a dynamically linked Linux binary looks like in Ghidra. The main focus is recognizing imported functions, following calls such as `strcmp` and `strlen`, and understanding the basic purpose of the PLT/GOT without memorizing loader internals.

## Source
The program reads a short word, compares it with `"reverse"`, prints whether it matched, and reports its length.

Compile:
```bash
gcc -g source/library_calls.c -o library_calls
```

Useful inspection commands:
```bash
file library_calls
ldd library_calls
objdump -T library_calls | grep -E 'strcmp|strlen|puts|printf'
```

## 1. Internal vs imported functions
A binary can contain its own functions while also using functions supplied by shared libraries. Calls such as `strcmp`, `strlen`, `printf`, and `puts` are normally resolved from libc in a dynamically linked build.

In reverse engineering, imported function names are valuable clues. Seeing `strcmp` strongly suggests comparison logic; `strlen` suggests string-length handling.

## 2. What are PLT and GOT?
On ELF systems, dynamically linked calls commonly involve the **Procedure Linkage Table (PLT)** and **Global Offset Table (GOT)**.

Beginner mental model:
```text
program code
   ↓ CALL
PLT entry
   ↓
GOT / dynamic linker information
   ↓
shared-library function
```

The exact generated code varies by compiler, linker, architecture and options.

## 3. Ghidra exercise
Open the compiled binary and locate `main`.

Try to identify:
- the local `input` array on the stack,
- the string `"reverse"`,
- the call related to `strcmp`,
- the branch after the comparison,
- calls to `puts`/`printf`,
- the call related to `strlen`.

Follow the `"reverse"` string reference. String references are often an easier entry point into unknown code than reading assembly line by line.

## 4. Comparison pattern
The source contains:
```c
if (strcmp(input, "reverse") == 0)
```

A common decompiler pattern is conceptually:
```text
result = strcmp(input, "reverse")
if result != 0 -> no-match path
else           -> match path
```

Remember: `strcmp` returning zero means the strings are equal.

## 5. Assembly pattern
Depending on the build, you may encounter a pattern similar to:
```asm
CALL strcmp
TEST EAX,EAX
JNZ  no_match
```

Do not memorize these exact instructions. Learn the higher-level pattern:
```text
call comparison function
      ↓
inspect return value
      ↓
conditional branch
```

## 6. Why imports matter
Imported functions can reveal behavior before you understand every instruction.

Examples:
- `strcmp` -> compare strings
- `strlen` -> measure string length
- `malloc` -> allocate heap memory
- `fopen` -> open a file
- `socket` -> create a socket

An import is a clue, not proof of malicious or benign behavior.

## Exercises
1. Find `main` and rename any unclear local variables.
2. Locate the `"reverse"` string and follow its cross-reference.
3. Find the comparison call and the conditional branch that follows it.
4. Find `strlen` and identify where its return value is used.
5. Compare Ghidra's decompiler with the original C source.
6. Inspect the imports and write down what each familiar function suggests.

## Questions
1. Why can imported function names help a reverse engineer?
2. What does `strcmp` return when two strings are equal?
3. What is the beginner-level purpose of the PLT/GOT?
4. Why is a string cross-reference useful?
5. Why can compiler/linker output differ from the examples?

## Main takeaway
A useful workflow is:
```text
strings/imports
     ↓
cross-references
     ↓
interesting calls
     ↓
return-value checks
     ↓
branches and behavior
```

This is much more practical than trying to understand every assembly instruction from top to bottom.
