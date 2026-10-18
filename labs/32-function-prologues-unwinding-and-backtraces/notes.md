# Day 32 — Function Prologues, Stack Unwinding and Backtrace Reliability

[English](notes.md) · [Türkçe](notes.tr.md)

## Learning goals
Learn the mechanics and security limits of **Function Prologues, Stack Unwinding and Backtrace Reliability** by reproducing a harmless local example. Today's work connects observation, policy, testing and evidence quality.

## Concepts
1. **Baseline:** establish a known-good observation before changing the example.
2. **Trust boundaries:** separate raw input, tool observations and interpretation.
3. **Environment dependencies:** identify how system versions, configuration and execution conditions change behavior.
4. **Regression:** rerun positive and negative cases after modifications.
5. **Evidence:** report what was observed, rather than assuming malicious activity.

## Reproducible lab
Run these from this folder:

```bash
gcc -g -O0 -fno-omit-frame-pointer unwind_demo.c -o frames
```

```bash
gcc -g -O2 -fomit-frame-pointer unwind_demo.c -o optimized
```

```bash
gdb -q ./frames
```

```bash
readelf --debug-dump=frames frames
```

Expected result: Compare backtrace and disassembly with and without frame pointers.

## Investigation tasks
1. Read the sample code and predict the output before running.
2. Execute it twice; record stable and variable fields.
3. Change one harmless input or setting. State your hypothesis, then compare results.
4. Introduce a safe negative scenario and confirm the guard or comparison behaves as expected.
5. Describe one false positive and one false negative relevant to the concept.
6. Identify at least one independent observation that would increase confidence.
7. Write a small report with method, evidence, uncertainty and recommended next test.

## Mini challenge
Rebuild the demonstration as a tiny automated test that fails when a known security property changes. Explain why a passing test does not imply the entire system is secure.

## Review questions
1. What is the security boundary in this example?
2. What does the program prove, and what is still unknown?
3. Which part is dependent on the OS, library or runtime environment?
4. What would an unexpected result mean?
5. How can an analyst reproduce and independently verify the finding?
6. Which safeguards are useful outside the lab?
7. Why must you avoid treating a single indicator as conclusive evidence?
8. What should be included in a regression test?

## Safety
Only local, synthetic and authorized targets. Do not treat the output as proof of maliciousness or system compromise.
