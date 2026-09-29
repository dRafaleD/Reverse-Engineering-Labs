# Day 20 — ELF Sections, Symbols and Stripped Binaries

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Understand how a Linux ELF binary is organized into sections, how symbols help reverse engineering, and what changes when a binary is stripped.

This lab connects C source code to common ELF sections such as `.text`, `.rodata`, `.data`, and `.bss`.

## Source

`source/elf_sections.c` contains:

- executable code,
- an initialized global,
- an uninitialized global,
- a read-only string,
- a static helper function.

Compile an unstripped build:

```bash
gcc -g -O0 source/elf_sections.c -o elf_sections
```

Create a stripped copy:

```bash
cp elf_sections elf_sections_stripped
strip elf_sections_stripped
```

## 1. What is ELF?

ELF stands for **Executable and Linkable Format**.

It is commonly used on Linux for:

- executables,
- shared libraries,
- object files,
- core dumps.

A reverse engineer does not need to memorize the whole ELF specification on Day 20. The goal is to understand that the binary contains structured regions with different purposes.

## 2. Important sections

Typical sections include:

```text
.text    -> executable machine code
.rodata  -> read-only constants and strings
.data    -> initialized writable global/static data
.bss     -> uninitialized or zero-initialized global/static data
```

Other sections exist, but these four are a strong starting point.

## 3. Map the C variables

The source contains:

```c
int initialized_global = 42;
int uninitialized_global;
static const char message[] = "ELF section training";
```

A useful beginner expectation is:

```text
initialized_global   -> .data
uninitialized_global -> .bss
message              -> .rodata
helper/main code     -> .text
```

Exact layout can vary with compiler/linker behavior, but the categories are useful.

## 4. Inspect sections

Run:

```bash
readelf -S elf_sections
```

or:

```bash
objdump -h elf_sections
```

Find:

- `.text`
- `.rodata`
- `.data`
- `.bss`

Record their addresses and sizes.

## 5. Inspect symbols

Run:

```bash
nm elf_sections
```

Look for:

```text
main
helper
initialized_global
uninitialized_global
```

Debug/unstripped binaries provide names that make analysis easier.

You can also try:

```bash
readelf -s elf_sections
```

## 6. What does strip do?

`strip` removes symbol/debug information that is not required for normal execution.

Compare:

```bash
nm elf_sections
nm elf_sections_stripped
```

The stripped binary still runs, but many helpful names may disappear.

This is important in reverse engineering because real-world binaries are often stripped.

## 7. Ghidra comparison

Import both binaries into separate Ghidra programs:

```text
elf_sections
elf_sections_stripped
```

Compare:

- function names,
- global names,
- strings,
- decompiler output,
- XREFs.

Even if symbols disappear, the machine code and many strings still remain.

## 8. Strings survive stripping

Try:

```bash
strings elf_sections_stripped | grep "ELF section"
```

Stripping symbols does not automatically remove literal strings.

That means strings can still provide useful entry points when symbol names are gone.

## 9. Sections vs segments

A beginner distinction:

```text
sections -> linker/static-analysis organization
segments -> loader/runtime mapping
```

Tools such as `readelf -S` show sections.

```bash
readelf -l elf_sections
```

shows program headers/segments.

You do not need to master this distinction yet; just recognize that section layout and runtime memory mapping are related but not identical concepts.

## 10. Security connection

When analyzing unknown Linux software, section and symbol information can help answer:

- Where is executable code?
- Where are constants and strings?
- Which global data is writable?
- Are useful function names still present?
- Has the binary been stripped?

A stripped binary is not automatically malicious. Stripping is common in normal release builds.

## Exercises

1. Compile the unstripped binary.
2. List sections with `readelf -S`.
3. Find the four main sections from this lab.
4. List symbols with `nm`.
5. Create a stripped copy.
6. Compare `nm` output before and after stripping.
7. Open both binaries in Ghidra and compare function names.
8. Find the `"ELF section training"` string in the stripped binary.
9. Follow its XREF to code that uses it.

## Questions

1. What is the purpose of `.text`?
2. Why would an initialized global commonly be in `.data`?
3. Why would an uninitialized global commonly be in `.bss`?
4. What kind of data commonly appears in `.rodata`?
5. What does stripping remove?
6. Can a stripped binary still run?
7. Why can strings remain useful after symbols are removed?
8. Are ELF sections and runtime segments exactly the same thing?

## Main takeaway

A useful static-analysis map is:

```text
ELF
 ├─ .text   -> code
 ├─ .rodata -> constants/strings
 ├─ .data   -> initialized writable globals
 └─ .bss    -> zero/uninitialized globals
```

Then add symbol awareness:

```text
symbols present -> easier navigation
symbols stripped -> rely more on strings, XREFs, control flow and behavior
```
