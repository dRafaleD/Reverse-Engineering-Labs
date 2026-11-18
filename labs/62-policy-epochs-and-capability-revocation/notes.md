# Day 62 — Policy Epochs and Capability Revocation

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](policy_epochs.c)

## After Day 61
Day 61 separated **handle identity** (generation) from **permitted operations** (read/write bits). Day 62 adds **revocation**. A permission can be removed while the object remains active, and later granted again. An old handle should not silently regain its former authority.

## Model and invariant
The permanently allocated `Slot` stores `generation`, `policy_epoch`, active state, allowed rights and a value. Each `Handle` captures generation, epoch and issued rights. Every read/write checks:
1. The slot exists and is active; generation matches.
2. The handle's **policy epoch** matches the current slot epoch.
3. Both issued rights **and** currently allowed rights permit the requested operation.

`set_policy()` increments the epoch. Revoking WRITE makes the existing writer stale. Restoring WRITE later does **not** revive that stale handle; a new one must be issued. Epoch overflow is rejected rather than wrapped.

## Reproduce
~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 policy_epochs.c -o policy_epochs
./policy_epochs
gdb -q ./policy_epochs
~~~
In GDB, break at `authorized` and compare the two policy-epoch fields. In Ghidra, locate the two independent equality checks (generation and epoch), then the permission bitmasks. Compare with `-O2`.

## Questions
Why can restoring a previously revoked permission be dangerous for old handles? How is generation different from policy epoch? What if the slot were actually freed? Would this be race-free with multiple threads?

## Scope
A safe, single-threaded pointer and authorization fixture, **not** a real OS capability system or safe general-purpose allocator.
