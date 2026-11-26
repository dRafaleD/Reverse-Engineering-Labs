# Day 71 — Transactional Record Parsing and Output Integrity

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](transactional_parser.c)

Day 70 examined record transitions. Today an invented byte stream contains a count and tag/value pairs. The parser builds a temporary summary and only publishes it after every record validates. A late invalid tag must leave the caller's original output untouched. The 11 cases cover empty and maximum records, truncated and trailing input, invalid tags and values, and null arguments. Inspect candidate and out in GDB before and after the final assignment. Compare the successful two-record summary (2,30) with the late-error sentinel (99,999). Add a valid zero-valued record and predict its summary. This is a single-threaded local fixture, not a production binary parser or a durable transaction.

## Run
```bash
cd labs/71-transactional-record-parsing
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 transactional_parser.c -o parser
./parser
```
