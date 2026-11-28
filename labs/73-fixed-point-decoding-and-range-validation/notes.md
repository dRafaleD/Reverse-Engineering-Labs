# Day 73 — Fixed-Point Decoding and Range Validation

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](fixed_point.c)

Day 72 covered explicit signed byte decoding. Today interpret a fictional two-byte little-endian temperature in tenths of a degree. Decode signed arithmetic before applying the permitted interval -400 to 1250. For example FA 00 means 25.0 degrees, FF FF means -0.1, 70 FE means -40.0. A rejected field leaves the output unchanged. Predict the nine value cases and three invalid argument cases. Inspect raw and value in GDB, then add 01 00 and predict its value. This is a local arithmetic fixture, not a real sensor protocol.

## Run
```bash
cd labs/73-fixed-point-decoding-and-range-validation
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 fixed_point.c -o fixed_point
./fixed_point
```
