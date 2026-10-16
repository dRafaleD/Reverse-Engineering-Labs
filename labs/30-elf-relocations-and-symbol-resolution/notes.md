# Day 30 — ELF Relocations, Symbols and Runtime Resolution

[English](notes.md) | [Türkçe](notes.tr.md)

## Objective

Build an authorized, local and repeatable practical exercise about elf relocations, symbols and runtime resolution. The lab intentionally uses harmless synthetic data. A tool observation is evidence of a particular execution, not proof of compromise.

## Connected concepts

1. **symbol tables** — identify its role, trust assumptions, and limitations in the workflow.
2. **relocations** — identify its role, trust assumptions, and limitations in the workflow.
3. **PIE/ASLR** — identify its role, trust assumptions, and limitations in the workflow.
4. **PLT/GOT** — identify its role, trust assumptions, and limitations in the workflow.
5. **readelf and Ghidra cross-check** — identify its role, trust assumptions, and limitations in the workflow.

## Setup and practice

Use the included `relocation_demo.c` as the locally controlled target. Do not test unrelated infrastructure or real suspicious artifacts.

```bash
gcc -g -O0 -fPIE -pie relocation_demo.c -o demo
```

```bash
readelf -rW demo
```

```bash
readelf -Ws demo
```

```bash
gdb ./demo
```

## Analysis workflow

1. Predict expected results before running the lab.
2. Collect actual outputs and note the tool/version/environment.
3. Separate observed facts from inferred explanations.
4. Introduce one harmless negative case, then rerun.
5. Explain whether a result could be a false positive, incomplete evidence, or a configuration issue.
6. Keep the original sample and record a checksum when preserving evidence.
7. Compare what static/configuration data says with the runtime observation.
8. Record a concise conclusion and at least one unresolved question.

## Mini challenges

- **Baseline:** run the provided example and write down exact output and assumptions.
- **Change one variable:** alter one benign marker, header, log field, or configuration setting, then record what changed.
- **Regression:** restore the original test and ensure that it still produces the expected observation.
- **Detection vs proof:** describe why your evidence does not by itself establish malicious activity.

## Review questions

1. What does symbol tables contribute, and what does it not guarantee?
2. What does relocations contribute, and what does it not guarantee?
3. What does PIE/ASLR contribute, and what does it not guarantee?
4. What does PLT/GOT contribute, and what does it not guarantee?
5. What does readelf and Ghidra cross-check contribute, and what does it not guarantee?
6. What independent evidence would strengthen this conclusion?
7. How could you reproduce the result next week?

## Safety and interpretation

This educational example is isolated, local-only and synthetic. Never infer safety from a missing match or compromise from an isolated suspicious indicator.