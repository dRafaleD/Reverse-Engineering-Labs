# Day 24 — System Calls, libc Wrappers, strace and ltrace

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Connect C library calls, imported functions, and the Linux kernel interface by observing the same harmless program from four perspectives:

1. source code,
2. Ghidra/static analysis,
3. `ltrace` library-call tracing,
4. `strace` system-call tracing.

This day combines:

- libc vs kernel,
- system calls,
- library wrappers,
- dynamic linking,
- file descriptors,
- process/file activity,
- tracing,
- return values and errors,
- static vs dynamic evidence,
- correlation between multiple tools.

The goal is to understand **which layer is doing what**.

## 1. Application, libc and kernel

A simplified Linux execution path:

```text
your C program
     ↓
libc / shared libraries
     ↓
system call boundary
     ↓
Linux kernel
     ↓
filesystem / process / device / network resources
```

Not every C function is itself a system call.

For example:

- `strlen()` is normally library logic,
- `malloc()` is allocator/library logic that may eventually request memory from the OS,
- `printf()` is a libc function,
- `write()` directly corresponds closely to a kernel system call interface,
- `open()` is commonly implemented through a libc wrapper around a kernel file-opening syscall such as `openat`.

## 2. Why reverse engineers care

Static analysis may show:

```text
CALL printf
CALL malloc
CALL open
CALL write
```

Dynamic tracing can show what actually happened at runtime.

This helps answer:

- Which file was opened?
- Which path was used?
- Which calls failed?
- Which file descriptor was returned?
- Which bytes were written?
- Which libraries were involved?

## 3. Build the target

Compile:

```bash
gcc -g -O0 source/syscall_trace.c -o syscall_trace
```

Run normally:

```bash
./syscall_trace
```

Expected behavior:

- prints a start message,
- prints its PID,
- allocates/copies a string,
- creates `day24_output.txt`,
- writes a harmless line,
- closes the file,
- frees memory,
- prints `Finished.`

## 4. Inspect imports first

Before running tracing tools:

```bash
objdump -T syscall_trace | less
```

or:

```bash
readelf -Ws syscall_trace | less
```

Look for imported symbols such as:

- `puts`
- `printf`
- `getpid`
- `malloc`
- `strlen`
- `strcpy`
- `open`
- `write`
- `close`
- `free`

This gives you a static hypothesis about behavior.

## 5. Ghidra view

Import the binary into Ghidra.

In `main`, identify:

```text
string output
PID lookup
heap allocation
string copy
file open
file write
file close
heap free
```

Use XREFs and imported functions.

Before tracing, write down what you predict the program will do.

## 6. What is a system call?

A system call is a controlled request from user space into the kernel.

Examples include operations related to:

- files,
- processes,
- memory mappings,
- networking,
- signals.

The kernel validates and performs privileged resource operations.

Conceptually:

```text
user mode
   ↓ syscall boundary
kernel mode
   ↓
resource operation
```

## 7. File descriptors

Linux represents many open resources using integer file descriptors.

Common defaults:

```text
0 -> stdin
1 -> stdout
2 -> stderr
```

When the training program opens a new file, it may receive descriptor 3 or another available integer.

Example trace:

```text
openat(...) = 3
write(3, ...) = 27
close(3) = 0
```

The exact descriptor can vary.

## 8. strace

Run:

```bash
strace ./syscall_trace
```

This shows system calls.

The output can be noisy because program startup involves many loader/library operations.

Filter to interesting calls:

```bash
strace -e trace=openat,write,close,getpid ./syscall_trace
```

Now focus on:

- PID query,
- file open,
- writes,
- file close.

## 9. Read a strace line

Example shape:

```text
openat(AT_FDCWD, "day24_output.txt", ..., 0644) = 3
```

Break it down:

```text
function/syscall-like operation
arguments
pathname
flags/mode
return value
```

Then:

```text
write(3, "Reverse Engineering Day 24\n", 27) = 27
```

Interpretation:

- descriptor 3,
- requested bytes,
- returned number of bytes written.

## 10. Return values and errors

Linux calls commonly signal failure with negative/error semantics surfaced by libc as:

```text
-1 + errno
```

For example, opening a missing/inaccessible path may yield:

```text
= -1 EACCES (...)
```

Dynamic traces make error paths visible.

A reverse engineer should pay attention to both success and failure behavior.

## 11. ltrace

If installed:

```bash
ltrace ./syscall_trace
```

`ltrace` focuses on dynamic library calls.

You may see calls such as:

- `puts`
- `printf`
- `malloc`
- `strlen`
- `strcpy`
- `free`

Exact output depends on environment, compiler, optimization, libc, and tracing support.

## 12. strace vs ltrace

A useful distinction:

```text
ltrace -> library-call view
strace -> system-call view
```

They overlap conceptually but observe different layers.

Example:

```text
printf()
   ↓ libc processing
write()
   ↓ kernel system call
```

A single high-level library call can cause one or more system calls.

## 13. malloc does not equal one fixed syscall

Do not assume:

```text
malloc() = brk()
```

or:

```text
malloc() = mmap()
```

Allocator behavior depends on:

- allocation size,
- allocator state,
- libc implementation,
- previous allocations.

You may observe `brk`, `mmap`, or no new memory-related syscall for a particular small allocation because memory was already available.

This is why dynamic observation matters.

## 14. open vs openat

Source code may call:

```c
open(...)
```

while `strace` shows:

```text
openat(...)
```

This is normal.

The libc wrapper and underlying kernel interface do not always share the exact same name.

Important lesson:

```text
source-level API != necessarily exact kernel syscall name
```

## 15. Static evidence vs runtime evidence

Static analysis can tell you:

> The binary imports `open`.

Runtime tracing can tell you:

> During this execution, it opened `day24_output.txt`.

These are different claims.

```text
static capability / possible path
        vs
observed runtime behavior
```

## 16. Correlate with Ghidra

Find the `open` call in Ghidra.

Trace backward:

- where does the filename pointer come from?
- what flags are passed?
- what mode is passed?

Then compare with `strace`.

This is the workflow:

```text
Ghidra hypothesis
      ↓
strace observation
      ↓
confirm argument meaning
```

## 17. File flags

The source uses:

```c
O_CREAT | O_WRONLY | O_TRUNC
```

Meaning:

- create if needed,
- open for writing,
- truncate existing content.

In assembly/decompiler output, these may appear as numeric constants or combined bitmasks.

This connects to Day 17 bitwise flags.

## 18. Process activity

The program calls:

```c
getpid()
```

This gives the current process ID.

Observe it in:

```bash
strace -e trace=getpid ./syscall_trace
```

Then compare to the printed PID.

This is a small example of correlating program output with kernel-visible behavior.

## 19. Loader noise

Full `strace` output includes activity before your `main()` logic:

- shared-library loading,
- memory mappings,
- locale/config reads,
- runtime initialization.

This is valuable evidence, but beginners should first filter to calls relevant to the lab.

Later, revisit the full trace and identify which activity belongs to program startup.

## 20. Mini challenge — predict before tracing

Before running `strace`, predict:

- which file path will be opened,
- whether it is read or write,
- whether a new file will be created,
- which string will be written,
- whether `close` will occur.

Then compare to the trace.

## 21. Mini challenge — induce an error safely

Create a directory:

```bash
mkdir readonly_dir
chmod 555 readonly_dir
```

Modify a **working copy** of the source so it tries to create:

```text
readonly_dir/day24_output.txt
```

Run under `strace`.

Observe the failed open result.

Restore permissions afterward:

```bash
chmod 755 readonly_dir
rmdir readonly_dir
```

Depending on privileges/environment, root may bypass ordinary permission behavior, so interpret your actual result.

## 22. Mini challenge — compare optimized build

Compile:

```bash
gcc -g -O2 source/syscall_trace.c -o syscall_trace_O2
```

Compare:

- Ghidra decompiler,
- imported symbols,
- `ltrace`,
- `strace`.

Some library calls may disappear or change due to compiler optimization.

Kernel-visible behavior may remain similar even when source/library-level structure changes.

## 23. Why this matters for malware analysis

The same methodology can later help analyze suspicious software safely in an isolated environment:

```text
static imports
   ↓
behavior hypothesis
   ↓
dynamic tracing
   ↓
filesystem/process/network evidence
```

But this repo continues to use harmless programs so you can learn the method before handling real malware.

## 24. Analysis worksheet

Create a table:

| Observation | Tool | Layer | Meaning |
| --- | --- | --- | --- |
| `open` imported | Ghidra/readelf | library/static | binary references file-opening API |
| `openat(...day24_output.txt...)=3` | strace | kernel/runtime | file opened in this execution |
| `malloc(...)` | ltrace | libc/runtime | allocator API called |
| `write(3,...)` | strace | kernel/runtime | bytes written to fd 3 |

Add at least six observations.

## Exercises

1. Compile and run the target.
2. Inspect dynamic imports.
3. Predict behavior from Ghidra.
4. Run full `strace`.
5. Run filtered `strace`.
6. Identify the file descriptor.
7. Explain the `open` vs `openat` difference.
8. Run `ltrace`.
9. Compare library calls with system calls.
10. Find file flags in Ghidra.
11. Perform the safe error-path challenge.
12. Compare `-O0` and `-O2`.
13. Complete the six-row worksheet.
14. Explain which observations are static and which are runtime.

## Questions

1. What is a system call?
2. What is libc?
3. Why is every libc call not a syscall?
4. What is a file descriptor?
5. strace vs ltrace?
6. Why can `open()` appear as `openat()`?
7. Why might `malloc()` not cause a new syscall every time?
8. What does a syscall return value tell you?
9. What is the difference between static capability and observed behavior?
10. Why is full strace output noisy?
11. How do bit flags appear in reverse engineering?
12. Why compare Ghidra with runtime traces?
13. How can optimization change ltrace observations?
14. Why should error paths also be analyzed?

## Main takeaway

```text
source
  ↓
libc/API calls
  ↓
system-call boundary
  ↓
kernel-visible behavior
  ↓
strace / ltrace
  ↓
correlate with Ghidra
```

Day 24 teaches you to move between abstraction layers instead of treating every function call as the same kind of event.
