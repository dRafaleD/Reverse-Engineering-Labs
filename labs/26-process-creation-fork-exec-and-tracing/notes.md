# Day 26 — Process Creation, fork/exec, waitpid and Runtime Process Tracing

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning goals

Connect **four closely related skills**: (1) process identity and process trees; (2) `fork()`, `execve()` and `waitpid()`; (3) static analysis versus `strace` and GDB observations; and (4) evidence from `/proc` and environment/argument handling. This continues Day 24 system calls and Day 25 ELF loading.

**Scope:** a harmless program you build yourself. Linux VM recommended; no malware or privileged actions.

## 1. Process, thread, executable: not synonyms

A file such as `day26` is an executable on disk. A running **process** has a PID, address space, file descriptor table, process credentials and execution state. Multiple processes may execute the same binary. Threads in one process share some resources but have distinct execution contexts; this lab is about **processes**, not thread creation.

```text
Parent process (PID P) -- fork --> Child process (PID C)
Parent waits                         |
                                     +-- exec --> /bin/echo (still PID C)
                                     |
                                  exits (status)
Parent waitpid returns  <-------------+
```

The exact PID numbers are assigned by Linux, not fixed in source code.

## 2. fork(): two execution paths

The source calls `fork()` once. On success, **two processes** continue after that call:

- Parent receives the **child PID** (positive).
- Child receives **0**.
- Failure returns **-1** in the original process.

A successful `fork` creates a separate process with its own virtual address space. Initial memory contents can appear identical, but changes are isolated; modern Linux commonly uses copy-on-write. Do not assume two independent physical copies of every page are made immediately.

### Buffering gotcha

Buffered stdout may duplicate pre-fork text if it wasn't flushed. Our target uses `setvbuf(stdout, NULL, _IONBF, 0)` to make the demonstration easier to interpret.

## 3. exec(): replace the current process image

The child calls `execl("/bin/echo", ...)`, one member of the libc **exec family**. Underneath, Linux performs an image replacement through `execve` (or related execution path). This does **not** create a third process or new PID; the child becomes the echo program.

Important differences:

| Operation | New process? | Same PID? | Returns on success? |
| --- | --- | --- | --- |
| `fork` | Yes | Parent retains its PID; child gets new PID | Yes, in both processes |
| `exec` | No | Yes | **No** |
| `waitpid` | No | Parent stays parent | Yes, after selected child state changes |

After successful `exec`, the earlier user-space code and address-space mappings are replaced. Descriptors marked close-on-exec are closed; other permitted descriptors can persist. The program's credentials and environment handling depend on the execution conditions. Do not assume `exec` resets everything.

## 4. waitpid(): observe completion, not guess it

The parent calls `waitpid(child, &status, 0)`. This waits for the specified child and reaps it, avoiding an unreaped zombie for this lab.

- `WIFEXITED(status)`: child terminated normally.
- `WEXITSTATUS(status)`: extract normal exit code **only if WIFEXITED is true**.
- `WIFSIGNALED(status)`: child died from a signal.
- `WTERMSIG(status)`: extract terminating signal only in that case.

A printed `exit status=0` is an observation about this execution, not proof that all operations in the program were secure.

## 5. Compile, run and inspect

From this lab directory:

```bash
gcc -std=c11 -g -O0 -Wall -Wextra -Wpedantic \
  source/process_lifecycle.c -o day26
./day26
```

Expected *shape* (PIDs and ordering of concurrent lines vary):

```text
PARENT before fork: pid=...
PARENT: child pid=...; waiting
CHILD before exec: pid=... ppid=...
DAY26_CHILD_EXEC_OK
PARENT: exit status=0
```

The initial parent/child messages can interleave differently. The parent's final wait result follows child termination.

## 6. Static hypothesis first: Ghidra and imported symbols

```bash
readelf -Ws day26 | grep -E 'fork|execl|waitpid|getpid|sleep'
objdump -d day26 | less
```

Import `day26` into Ghidra and locate `main`, then annotate: `getpid`, `fork`, the `child == 0` branch, `sleep`, `execl`, `waitpid` and status checks. Calls in the decompiler are **evidence of possible paths**; they do not prove all paths executed.

Find XREFs to `DAY26_CHILD_EXEC_OK`. Why does the string exist in the parent's executable even though a different program eventually prints it?

## 7. Dynamic proof: strace follows children

```bash
strace -f -e trace=process ./day26
```

For per-process files:

```bash
strace -ff -o day26.trace ./day26
ls day26.trace.*
```

Look for:

- the program's initial `execve`;
- a `clone`-family call corresponding to libc `fork()` on this Linux/glibc combination (or a direct `fork` on another environment);
- child's `execve("/bin/echo", ...)`;
- parent's `wait4` / equivalent wait-family syscall;
- child termination and parent's result.

**Don't assume source API names must match syscall names**: similar to `open()` appearing as `openat()` on Day 24. Exact trace output is system-dependent.

## 8. GDB: follow the fork and catch exec

```bash
gdb ./day26
```

Inside GDB:

```text
set follow-fork-mode child
set detach-on-fork off
catch fork
catch exec
break main
run
info inferiors
continue
```

`info inferiors` shows debugger-tracked processes. After the fork catchpoint, use `info inferiors`, `inferior N`, `bt` and `info registers` to compare parent and child. The exact stopping sequence depends on GDB and glibc.

After successful exec, the child is in **a different program image**. Inspect mappings and note why breakpoints in the original program may not behave the same way.

## 9. /proc and observation limits

The child sleeps briefly before exec so you can inspect it from a second terminal:

```bash
ps -eo pid,ppid,stat,comm | grep -E 'day26|echo'
cat /proc/<CHILD_PID>/status
readlink /proc/<CHILD_PID>/exe
tr '\0' ' ' < /proc/<CHILD_PID>/cmdline
```

Replace `<CHILD_PID>` with the actual child PID. Processes can exit before you inspect them; a missing `/proc` entry is often a timing issue, not suspicious evidence. Prefer repeating the lab rather than inventing observations. Some proc entries are permission-restricted.

**Security connection:** command-line arguments and environment values can be exposed through process inspection depending on permissions. Do not use real passwords or API tokens in this lab.

## 10. Process trees, exec and the loader

Before exec, the child is running code from `day26`. After exec, it loads `/bin/echo`, its own ELF interpreter and shared libraries. Connect this to Day 25: the dynamic loader runs as part of making the new executable usable.

Conceptual evidence chain:

```text
Ghidra (fork + execl imports)
      ↓ hypothesis
strace (process creation + execve)
      ↓ observed events
GDB (/proc mappings + PID)
      ↓ runtime identity
waitpid (child termination)
```

One tool rarely tells the entire story.

## 11. Troubleshooting

- **Compiler error:** ensure Linux and GCC with POSIX APIs.
- **`strace: command not found`:** install strace through your VM's package manager.
- **`ptrace: Operation not permitted`:** check VM/container debugging permissions; avoid modifying production security settings for a lab.
- **No child in `ps`:** the 2-second window ended; use GDB or extend sleep on your own copy.
- **`execl` failure:** check `/bin/echo`; the child prints a `perror` message and exits 127.

## 12. Mini challenges

1. **Failure path:** on a lab copy, use a nonexistent exec path. Confirm `perror` runs and waitpid reports exit 127. Why does it execute only on failure?
2. **Execution image:** use `readlink /proc/<PID>/exe` before and after exec using GDB stops. Explain why the PID remains the same.
3. **File descriptors:** extend source with a harmless opened file and compare an ordinary descriptor with one marked `FD_CLOEXEC`; verify across exec with `strace`.
4. **Process tree:** draw parent/child PID relationships and mark which process produces every output line.

## Exercises

1. Compile with debug symbols and warnings.
2. Predict how many processes exist after fork.
3. Run five times; compare PIDs and ordering.
4. Find the fork branch in Ghidra.
5. Trace arguments to execl.
6. Identify the child status handling branch.
7. Run `strace -f`.
8. Separate trace files with `-ff`.
9. Track the PID across exec.
10. Revisit ELF loading from Day 25.
11. Explain stdout buffering across fork.
12. Write a 5–8 line static-versus-dynamic analysis summary.

## Review questions

1. Process versus executable?
2. fork return values in parent and child?
3. Does exec create a process?
4. What happens to PID across exec?
5. Why can fork use copy-on-write?
6. When does execl return?
7. What does waitpid achieve?
8. Why check WIFEXITED before WEXITSTATUS?
9. Why can source fork look like clone in strace?
10. How can GDB follow the child?
11. Why are /proc observations time-sensitive?
12. Can an imported API prove runtime execution?
13. What happens to inherited file descriptors on exec?
14. Why avoid secrets in argv/environment?
15. What other evidence supports an observed child process?

## Main takeaway

**fork creates a new process; exec replaces an existing process's program image; waitpid observes and reaps a child.** Static analysis proposes an explanation and runtime tracing tests it.
