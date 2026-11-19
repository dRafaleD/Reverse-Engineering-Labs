# Day 63 — Capability Attenuation and Safe Delegation

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](delegation.c)

## After Day 62
Day 62 used a policy epoch to revoke stale handles. Day 63 covers **attenuation**: a handle with read/write permission may create a *weaker* read-only handle, but a read-only handle must never delegate additional write permission.

## Model
- **Capability:** a reference with attached permissions.
- **Attenuation:** delegating only a subset of existing rights.
- **No amplification:** the requested rights must be a subset of the parent handle's rights.
- **Revocation:** both parent and delegated handles carry generation and policy epoch; old epochs fail once policy changes.

The C fixture creates a full-access handle, derives a read-only handle, rejects privilege expansion and invalid permission bits, then changes the policy epoch to invalidate all old handles. Its checks precede every read or write.

## Reproduce
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 delegation.c -o delegation
./delegation
gdb -q ./delegation
```

Break at `delegate` and `valid`, then compare `rights`, `allowed_rights` and `policy_epoch`. In Ghidra, find the bitwise subset test `(subset & parent->rights) == subset`.

## Challenges
Why is checking only the parent's rights insufficient after a policy change? Why does a read-only child not gain write access? How can policy-epoch rollover threaten revocation? What happens if the underlying slot is freed or used concurrently?

## Scope
Single-threaded, permanently allocated harmless C object. Not a kernel capability implementation, secure allocator or race-free authorization mechanism.
