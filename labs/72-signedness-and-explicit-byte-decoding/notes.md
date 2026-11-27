# Day 72 — Signedness and Explicit Byte Decoding

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](decode.c)

A byte is a bit pattern, not inherently a signed integer. Decode a fictional signed eight-bit field with explicit arithmetic rather than relying on implementation-defined conversions. The second function combines two bytes into an unsigned little-endian field. Worked examples: 0x7f → 127, 0x80 → -128, 0xff → -1; 34 12 → 0x1234. Predict all ten checks, compile at -O0 and -O2, and compare the conditional arithmetic in a debugger. Add a 0x81 case to a fresh test. The two-byte decoder assumes a valid two-byte array supplied by the caller; it is not a general parser or memory safety boundary.

## Run
```bash
cd labs/72-signedness-and-explicit-byte-decoding
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 decode.c -o decode
./decode
```
