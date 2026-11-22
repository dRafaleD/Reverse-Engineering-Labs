# Day 66 — Branch-Scoped Capability Revocation

[English](notes.md) · [Türkçe](notes.tr.md) · [C exercise](branch_revoke.c)

## After Day 65
The previous lab revoked a whole delegation family by changing a **resource-wide epoch**. But what if a service must revoke only one branch without interrupting a separate, still-authorized branch?

## Model
Two permanently allocated `Branch` objects point to the same toy `Resource`. Each branch maintains its own **revocation epoch**. Handles carry the resource identity, generation, branch pointer and captured epoch. Permission and identity checks occur **on every access**.

A left-branch epoch change invalidates its root and delegated reader while the right-branch reader remains usable. Changing the shared resource's **generation** later invalidates both branches. Delegation cannot amplify rights.

## Run
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -O0 -g branch_revoke.c -o branch_revoke
./branch_revoke
gdb -q ./branch_revoke
```

## Investigate
Why can a branch-specific revocation leave an independent branch valid? How is a resource generation different from a branch epoch? Why must the branch object outlive its handles? How could nested subtree revocation or concurrency complicate this toy design?

## Limits
Single-threaded, in-memory demonstration. Branch identity is a pointer, not an unforgeable token; this does not establish a real-world capability or thread-safe security boundary.
