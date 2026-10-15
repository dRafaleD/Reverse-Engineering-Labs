# Day 28 — File Formats, Magic Bytes, Endianness and Parser Tracing

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

This hands-on day builds on earlier labs, using only local harmless training data. Verify assumptions, distinguish observation from inference, and do not treat tool output as automatic security proof.

## Learning objectives

- Magic and byte-level contracts
- Little-endian length
- Boundary checks
- Static call graph
- Dynamic tracing
- Optimization effects
- Trust and limitations
- Analysis evidence

## Data flow and threat model

```text
controlled training input
        ↓
parser / evaluator / analyst
        ↓
security or evidence check
        ↓
observable result
        ↓
review assumptions and limits
```

## 1. Magic and byte-level contracts

Compare declared RE28 header with actual bytes, not the file extension.

**Task:** Explain which trust-boundary mistake this control prevents, with a concrete example.

## 2. Little-endian length

Bytes 03 00 encode payload length 3. Explain how the parser reconstructs the integer.

**Task:** Explain which trust-boundary mistake this control prevents, with a concrete example.

## 3. Boundary checks

Header requires six bytes; declared length must equal the bytes remaining. Test malformed files.

**Task:** Explain which trust-boundary mistake this control prevents, with a concrete example.

## 4. Static call graph

In Ghidra trace main, fopen, fread, memcmp, parse and return branches.

**Task:** Explain which trust-boundary mistake this control prevents, with a concrete example.

## 5. Dynamic tracing

In GDB break parse; inspect pointer, length and return value. Compare valid and truncated samples.

**Task:** Explain which trust-boundary mistake this control prevents, with a concrete example.

## 6. Optimization effects

Build with -O2 and compare decompiled code; source-level functions may inline.

**Task:** Explain which trust-boundary mistake this control prevents, with a concrete example.

## 7. Trust and limitations

A matching signature is not complete validation or proof that the input is safe.

**Task:** Explain which trust-boundary mistake this control prevents, with a concrete example.

## 8. Analysis evidence

Record input hash, compiler flags, tool version and which claim came from static vs runtime analysis.

**Task:** Explain which trust-boundary mistake this control prevents, with a concrete example.

## Local exercise

Use only the files in this repository. This example is not a production security service.

```bash
gcc -Wall -Wextra -g -O0 source/parser.c -o parser
python3 -c "from pathlib import Path;Path('valid.bin').write_bytes(b'RE28'+bytes([3,0])+b'ABC')"
./parser valid.bin
xxd valid.bin
gdb ./parser
```

## Test matrix

| Scenario | Expected | Reason |
|---|---|---|
| Valid training input | Accept / match | Baseline |
| Wrong owner or header | Reject / no match | Scope / structure |
| Truncated or missing field | Controlled failure | Input validation |
| Unrelated benign sample | No accidental claim | False-positive awareness |

## Exercises

1. Run and record the example.
2. Write an initial hypothesis.
3. Verify the positive case.
4. Add one negative case.
5. Add another negative case.
6. Test a boundary value.
7. Find the responsible source-level check.
8. Separate observations from inferences.
9. Change one control in a working copy.
10. Rerun the whole test matrix.
11. Document limitations.
12. Write a one-page analysis report.

## Review questions

1. What is the primary trust boundary?
2. Which inputs are untrusted?
3. What does the positive test establish?
4. What does the negative test establish?
5. What does the observed output support?
6. What does it not support?
7. Which input is the most important?
8. Give an example boundary value.
9. What is fail-closed behavior?
10. Why is the demo not production-ready?
11. Describe one potential false positive.
12. Describe one potential false negative.
13. Which fields belong in an audit log?
14. What personal data is unnecessary?
15. Why record tool versions?
16. Which regressions need retesting?

## Mini challenges

1. Add a new negative test and document its expected and actual results.
2. Break one assumption in a working copy and identify which test catches it.
3. Write a short report containing source, hash, tool version, command, observation and limitations.

## Takeaway

Measure actual behavior rather than overgeneralizing from synthetic cases. Security decisions, authorization and forensic inferences require independent checks.
