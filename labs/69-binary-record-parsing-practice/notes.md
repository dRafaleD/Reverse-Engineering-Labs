# Day 69 — Practice Day: Binary Record Parsing and Bounds Checks

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](record_parser.c)

Today's practice moves from capability objects to safe inspection of a tiny invented binary format. The six-byte header contains magic LB, version 1, type 1 or 2, and a two-byte little-endian payload length. Payloads are limited to eight bytes. The parser checks the header before reading fields and verifies exact size before returning a length.

| Worked input (hex) | Expected | Reason |
|---|---|---|
| 4c 42 01 01 00 00 | OK | Empty payload |
| 4c 42 01 02 01 00 2a | OK | One-byte payload |
| 4c 42 01 01 00 01 | BAD_LENGTH | Little-endian 256 |
| 4c 42 01 01 02 00 2a | TRUNCATED | One byte missing |
| 4c 42 01 01 00 00 2a | TRAILING | Unexpected byte |

**Practical tasks:** (1) Predict all 14 outcomes before running. (2) Break in parse in GDB and inspect the six header bytes. (3) Compile with -O2 and compare the length checks in Ghidra. (4) Add a valid two-byte example. (5) Verify that a failed parse leaves the output unchanged.

Expected last line: 14/14 cases passed. This fixture only parses local byte arrays; it is not a real file-format parser or a security boundary. Compile without NDEBUG to retain assertions.

## Run
```bash
cd labs/69-binary-record-parsing-practice
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 record_parser.c -o record_parser
./record_parser
```
