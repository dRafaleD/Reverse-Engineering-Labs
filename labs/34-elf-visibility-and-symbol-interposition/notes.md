# Day 34 — ELF Symbol Visibility and Shared Library ABI

[English](notes.md) | [Türkçe](notes.tr.md)

## Learning objective
Study visibility, symbol tables, linkage, ABI stability and static-versus-dynamic observations.

This controlled lab uses only harmless local samples. Record what the tools actually show and keep interpretation separate from evidence.

## Concept map
1. **Input and boundary:** identify who controls each input and which assumptions the implementation trusts.
2. **Mechanism:** trace the exact condition or action evaluated by the demonstration.
3. **Expected result:** predict the outcome before running the code.
4. **Failure path:** identify a safe negative case and why it differs from a valid case.
5. **Reproducibility:** record tool versions, command lines, inputs and results.
6. **Limitations:** distinguish educational simulation from a production-ready security control.

## Local exercise
From this lab directory:

```bash
gcc -g -O0 -fvisibility=hidden visibility.c -o visibility_demo
```

```bash
./visibility_demo
```

```bash
readelf -Ws visibility_demo | grep add
```

```bash
objdump -d visibility_demo | less
```

## Hands-on analysis
1. Read the source line by line and write a prediction.
2. Execute the program and capture stdout.
3. Identify each trust boundary; point to the exact line enforcing it.
4. Change one harmless value and compare behavior.
5. Add a negative regression test that would fail under a weak implementation.
6. Explain a realistic false-positive or false-negative interpretation.
7. Record a short finding with evidence, uncertainty and a verification plan.

## Mini challenge
Extend the demonstration with an automated assertion for one property, then design a second test intended to fail if that property is weakened.

## Review questions
1. What is the main security or analysis concept?
2. Which input or artifact is untrusted?
3. What exactly does the provided test establish?
4. What does the test *not* establish?
5. Which environmental factor could change the result?
6. Why is a negative test important?
7. Which second source of evidence could corroborate the result?
8. What must a professional report disclose?

## Safety
Exercise only on files or infrastructure you own or are explicitly authorized to inspect. Do not infer maliciousness from one indicator.