# Day 27 — Signals, Crash Analysis, Core Dumps and GDB Backtraces

[English](notes.md) · [Türkçe](notes.tr.md)

**Goal:** extend Day 26 process lifecycle into signals, deliberate safe crashes, GDB stack traces and evidence-based triage.

Prerequisites: Linux VM, GCC, GDB; optional Ghidra and strace. All activity stays on a harmless binary you compile.

## 1. Signals are asynchronous notifications

A signal informs a process about an event or request. Delivery and disposition depend on the OS and application. Some signals have default termination behavior; others may be caught or ignored. Signals are not equivalent to exceptions in every language.

## 2. SIGUSR1 versus SIGSEGV

SIGUSR1 is a user-defined signal that can be handled. SIGSEGV normally terminates a process when unhandled. In our target, SIGSEGV comes from raise(), deliberately, rather than an actual invalid memory access. Therefore the signal type alone does not prove memory corruption.

## 3. Handlers and async-signal-safety

Signal handlers execute in a constrained environment. Many standard-library functions, including ordinary formatted printing, must not be assumed async-signal-safe. This demo only assigns a volatile sig_atomic_t flag inside the handler; normal code prints afterward. This flag technique demonstrates a basic single-threaded notification, not general synchronization.

## 4. Call stack and backtrace

stage_one calls stage_two, which calls stage_three, which calls raise(SIGSEGV). Without aggressive optimization, a debugger can often display this chain. Stack frames, return addresses and debug symbols make it possible to identify the route to failure, not necessarily the root cause.

## 5. Build variants and symbol quality

With -g, GDB sees source lines and function symbols more easily. With -O2, functions may be inlined or otherwise transformed; backtraces can change. A stripped binary removes some useful symbols, but a signal and register state may still be observable. Preserve the original artifact and compare copies.

## 6. Core dumps and evidence care

A core dump is a process-memory snapshot created under suitable kernel and resource-limit settings; it can contain credentials or private data. It may be disabled or directed to a system crash collector. Prefer GDB's live crash stop for this lab; only enable core dumping in an isolated VM and never publish a real core file.

## 7. Static versus dynamic evidence

A Ghidra view can locate sigaction and raise imports, string mode checks, and functions. GDB confirms which mode executed and where the process stopped. strace -e trace=signal shows signal-related kernel observations. Distinguish possible code paths from behavior recorded in one run.

## 8. Crash triage before conclusions

Record the input mode, binary SHA-256, command, runtime environment, signal, current frame, full backtrace and disassembly. A crash report should specify what is observed and what remains unknown. SIGSEGV does not inherently imply exploitability.

## 9. Exit codes and parent observation

Shells frequently encode death by signal as a nonzero status, often 128 plus the signal number, but presentation is shell-specific. The deterministic comparison is whether normal/signal modes finish and crash mode ends via signal. Connect this to Day 26 waitpid and WIFSIGNALED.

## 10. Responsible reproduction

A good reproduction is small and safe: known local target, exact compiler flags, repeatable mode, no production host, no real secrets. Use the provided raise(SIGSEGV) rather than experimenting with uncontrolled memory corruption.

## Local lab: build and reproduce

From the lab directory:

```bash
gcc -std=c11 -g -O0 -fno-omit-frame-pointer -Wall -Wextra \
  source/signal_lab.c -o signal_lab
./signal_lab normal
./signal_lab signal
```

Normal completes successfully; signal installs a handler, sets a flag and continues. **Run the crash mode under GDB:**

```bash
gdb --args ./signal_lab crash
```

In GDB:

```text
set pagination off
break stage_three
run
continue
bt
frame 1
info registers
x/8i $pc
```

Continue reaches deliberate SIGSEGV. Record the stack chain stage_three → stage_two → stage_one → main. If the trace includes raise/libc frames, identify which are library frames and which belong to your program. Quit with `quit`.

### Static and runtime cross-check

```bash
readelf -Ws signal_lab | grep -E 'raise|sigaction|stage_'
strace -e trace=signal ./signal_lab signal
sha256sum signal_lab
```

Use Ghidra to inspect call XREFs to stage_three and control-flow branches. An import predicts possible behavior, not what occurred in the selected mode.

### Optional core-dump context

```bash
ulimit -c
cat /proc/sys/kernel/core_pattern
```

Core-dump availability depends on system settings and collectors. Never upload memory dumps from real systems or use secrets in this exercise.

## Mini challenges

1. Build a mode/observed-signal/result table for normal, signal and crash.
2. Compile a second `-O2` build and compare backtraces; note where optimization removes or changes frames.
3. In a personal working copy, remove the handler's flag assignment and describe how output differs.
4. Reconstruct the call chain as pseudocode from Ghidra without opening the source.

## Exercises

1. Explain every compiler flag.
2. Observe all three modes safely.
3. Find sigaction in imports.
4. Explain volatile sig_atomic_t.
5. Set a GDB breakpoint.
6. Record a backtrace.
7. Inspect program counter.
8. Draw the call chain.
9. Follow Ghidra XREFs.
10. Inspect signal events in strace.
11. Hash the compiled binary.
12. Write a short crash triage report.

## Review questions

1. What is a signal?
2. SIGUSR1 versus SIGSEGV?
3. Does SIGSEGV imply memory corruption?
4. Why avoid printf in a handler?
5. What is sig_atomic_t?
6. What does raise do?
7. What does a backtrace prove?
8. Why compile with -g?
9. Why can -O2 affect frames?
10. What can a core dump expose?
11. What controls core dump location?
12. Is a static import runtime proof?
13. Signal termination versus normal exit?
14. How is waitpid relevant?
15. What limitations belong in a crash report?

**Takeaway:** A signal is an observation, not a complete explanation; reconstruct the chain with static analysis, debugger state and corroborating runtime evidence.
