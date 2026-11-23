# Day 67 — Hierarchical Revocation and Ancestor Epochs

[English](notes.md) · [Türkçe](notes.tr.md) · [C fixture](ancestor_epochs.c)

## After Day 66
A branch-specific epoch revoked one capability family without affecting a sibling. Day 67 adds **nested branches** and an *ancestor check*: the state of every ancestor matters, not only the leaf issuing the final handle.

## Model
A root creates a left and right branch. The left branch delegates to a grandchild. Each branch records its parent's epoch at the time of creation, and each handle records its own branch epoch. To read, the fixture checks resource generation, rights, the current branch epoch and the captured epochs on the ancestor path.

Changing **left.epoch** invalidates left and its grandchild while the right sibling remains valid. Changing **root.epoch** then invalidates both subtrees. Merely reissuing a left handle does not restore an older grandchild branch; its parent-epoch binding also needs reissuance.

## Run
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 ancestor_epochs.c -o ancestor_epochs
./ancestor_epochs
gdb -q ./ancestor_epochs
```

## Questions
Why must descendant validation include ancestor state? Why does revoking an intermediate branch not revoke a sibling? What could happen if a branch pointer were freed or cyclic? Why would concurrent changes require synchronization?

## Limits
A single-threaded allocated-object teaching fixture, not an unforgeable OS capability implementation. The four-hop limit is illustrative; changing object fields directly is only safe within this controlled example.
