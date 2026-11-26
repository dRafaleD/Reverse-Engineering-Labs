# Day 70 — Record State Machines and Transition Validation

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](state_machine.c)

Day 69 validated individual binary records. A structurally valid record may still be invalid in the current session state. This fixture separates the event code from the allowed state transitions. EMPTY accepts BEGIN, OPEN accepts up to three DATA events and END, and CLOSED accepts nothing. Rejected transitions leave state and count unchanged.

| State / event | Expected |
|---|---|
| EMPTY / DATA | Reject |
| EMPTY / BEGIN | OPEN |
| OPEN / BEGIN | Reject |
| OPEN / fourth DATA | Reject |
| OPEN / END | CLOSED |
| CLOSED / BEGIN | Reject |

Compile with -O0, break at apply in GDB, and trace the switch branches. Compare the optimized build with -O2 in Ghidra. Add a separate empty-session test (BEGIN then END); explain why a parser and a state machine enforce different invariants. Expected final line: 13 transition scenarios passed. This is an invented local format, not a real protocol or OS security boundary.

## Run
```bash
cd labs/70-record-state-machines-and-transition-validation
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 state_machine.c -o state_machine
./state_machine
```
