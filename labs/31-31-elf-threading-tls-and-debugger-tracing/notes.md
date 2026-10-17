# Day 31 — Threads, Thread-Local Storage and GDB Thread Tracing

[English](notes.md) · [Türkçe](notes.tr.md)

## Goal
Understand threads, thread-local storage and gdb thread tracing through a fully local, harmless experiment. Keep observation separate from inference.

## Core concepts
1. **pthread_create and pthread_join** — document the boundary, expected behavior and limitations.
2. **thread-local storage** — document the boundary, expected behavior and limitations.
3. **shared versus thread-local memory** — document the boundary, expected behavior and limitations.
4. **synchronization and data races** — document the boundary, expected behavior and limitations.
5. **GDB info threads** — document the boundary, expected behavior and limitations.
6. **compiler and disassembly correlation** — document the boundary, expected behavior and limitations.

## Procedure
The included `threads.c` is a synthetic training example. From the lab directory:

```bash
gcc -g -O0 -pthread threads.c -o threads
```

```bash
./threads
```

```bash
gdb -q ./threads
```

```bash
readelf -sW threads | grep -E 'TLS|worker'
```

**Expected:** Each worker prints local=3; shared_total=6, but thread print order may vary.

## Observations to record
Write down the machine/tool versions, exact command, input, actual output, unexpected behavior, and interpretation. Explain which results are facts and which remain hypotheses.

## Exercises
1. Run the baseline and save the observed output.
2. Change one harmless input and predict what happens before running.
3. Add a negative test and distinguish a rejection from a failure to observe.
4. Restore the original, rerun it, and explain why regression tests matter.
5. Identify two realistic false-positive explanations.
6. Identify a useful independent corroborating observation.
7. Draft a short report stating limitations explicitly.

## Review questions
1. What does pthread_create and pthread_join mean, why does it matter, and what does it not guarantee?
2. What does thread-local storage mean, why does it matter, and what does it not guarantee?
3. What does shared versus thread-local memory mean, why does it matter, and what does it not guarantee?
4. What does synchronization and data races mean, why does it matter, and what does it not guarantee?
5. What does GDB info threads mean, why does it matter, and what does it not guarantee?
6. What does compiler and disassembly correlation mean, why does it matter, and what does it not guarantee?
7. What would improve the confidence of your result?
8. Which regression test should be automated first?

## Ethics and scope
Use only locally generated data and infrastructure you own or are authorized to study. Findings from an individual signal are not proof of an incident.
