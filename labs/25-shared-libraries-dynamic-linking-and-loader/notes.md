# Day 25 — Shared Libraries, Dynamic Linking, Loader Resolution and LD_DEBUG

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Goal

Understand how a Linux executable finds and uses code from a shared library at runtime.

This day combines shared objects, dynamic linking, imported/exported symbols, runtime loader behavior, library search paths, PLT/GOT review, ldd, readelf, nm, LD_DEBUG, relocations, ABI concepts, and Ghidra + GDB correlation.

## 1. Static vs dynamic linking

Conceptually:

~~~text
executable
   ↓ needs symbol
shared library
   ↓
runtime loader resolves it
~~~

A dynamically linked executable relies on shared objects at runtime instead of containing all dependency code itself.

## 2. Shared object

Linux shared libraries commonly use the .so format.

This lab builds:

~~~text
libday25.so
~~~

It exports two harmless functions:

- add_bonus
- print_banner

The executable imports them.

## 3. Build the shared library

~~~bash
gcc -fPIC -shared source/libday25.c -o libday25.so
~~~

-fPIC creates position-independent code.

-shared tells the linker to create a shared object.

## 4. Build the executable

~~~bash
gcc -g -O0 source/main.c -L. -lday25 -o day25_app
~~~

-L. adds the current directory to the link-time search path.

-lday25 links against libday25.so.

Important:

~~~text
link-time search path != runtime search path
~~~

The program can link successfully but still fail when executed if the runtime loader cannot find the library.

## 5. Observe runtime lookup

Try:

~~~bash
./day25_app
~~~

If the library is not found, use this local lab command:

~~~bash
LD_LIBRARY_PATH=. ./day25_app
~~~

Expected output:

~~~text
Day 25 shared library loaded.
input=10 result=17
~~~

## 6. ldd

Inspect dependencies:

~~~bash
ldd ./day25_app
~~~

Then:

~~~bash
LD_LIBRARY_PATH=. ldd ./day25_app
~~~

This helps answer which shared libraries the executable expects and where they resolve.

## 7. Dynamic section

Inspect:

~~~bash
readelf -d day25_app
~~~

Look for NEEDED entries.

You should see libday25.so and likely libc.

These entries are static metadata describing runtime dependencies.

## 8. Exported symbols

Inspect the library:

~~~bash
nm -D libday25.so
~~~

or:

~~~bash
readelf -Ws libday25.so
~~~

Find add_bonus and print_banner.

## 9. Imported symbols

Inspect the executable:

~~~bash
nm -D day25_app
~~~

Conceptually:

~~~text
day25_app needs add_bonus
libday25.so provides add_bonus
~~~

Dynamic linking connects them.

## 10. Runtime loader

The loader must:

- locate required libraries,
- map them into memory,
- process relocations,
- resolve symbols,
- transfer control into the program.

This directly extends the ELF and process-memory labs.

## 11. LD_DEBUG

Show library search activity:

~~~bash
LD_DEBUG=libs LD_LIBRARY_PATH=. ./day25_app
~~~

Show symbol binding activity:

~~~bash
LD_DEBUG=bindings LD_LIBRARY_PATH=. ./day25_app
~~~

The output can be noisy. Focus on the library search and binding decisions.

## 12. Search path thinking

Runtime library resolution may involve:

- executable RPATH/RUNPATH metadata,
- environment configuration,
- loader cache,
- default library directories.

For this beginner lab, focus on the local library plus LD_LIBRARY_PATH.

## 13. PLT/GOT connection

Day 18 introduced PLT/GOT.

A simplified external call is:

~~~text
main
  ↓
PLT entry
  ↓
GOT / resolver
  ↓
shared-library function
~~~

Open day25_app in Ghidra and inspect calls to add_bonus and print_banner.

## 14. Lazy binding

Some symbol resolution can happen on first use.

Conceptually:

~~~text
first call
   ↓
resolver
   ↓
target stored
   ↓
later calls reuse the resolved target
~~~

Exact behavior depends on linker/loader settings.

## 15. Relocations

Inspect:

~~~bash
readelf -r day25_app
readelf -r libday25.so
~~~

Core idea:

~~~text
build-time references
      ↓
runtime mapping
      ↓
loader adjusts addresses
~~~

Do not memorize relocation types yet.

## 16. Runtime mappings

Start GDB:

~~~bash
gdb ./day25_app
~~~

Then:

~~~text
set environment LD_LIBRARY_PATH .
break main
run
info proc mappings
~~~

Find:

- day25_app,
- libday25.so,
- libc,
- dynamic loader.

This connects directly to Day 22.

## 17. Break inside the library

With symbols available:

~~~text
break add_bonus
continue
info args
backtrace
x/i $rip
~~~

Now execution is paused inside code mapped from another ELF file.

## 18. Multiple ELF files, one process

On disk:

~~~text
day25_app
libday25.so
~~~

At runtime:

~~~text
one process address space
  ├─ executable mappings
  ├─ libday25.so
  ├─ libc
  └─ loader
~~~

This is the mental model to keep.

## 19. ABI compatibility

Dynamic linking relies on compatible expectations:

- symbol names,
- calling convention,
- parameters,
- return values,
- binary interface.

If the executable and library disagree, behavior may fail or become undefined.

This is an ABI problem.

## 20. Harmless alternate library challenge

Create a working copy of the library where add_bonus returns value + 100 instead of value + 7.

Build it locally and compare behavior.

Do not replace system libraries.

The purpose is to see that runtime behavior depends on the compatible implementation that gets loaded.

## 21. Security relevance

Dynamic linking matters in analysis because you may need to determine:

- which library actually loaded,
- which path it came from,
- which symbols were resolved,
- whether runtime behavior matches static assumptions.

The focus here is provenance and resolution, not hijacking real systems.

## 22. Mini challenge — dependency diagnosis

Without reading the answer first, determine:

1. why the executable fails without the local library path,
2. which command shows the missing dependency,
3. which command shows the loader search,
4. how the local lab makes the dependency resolvable.

## 23. Mini challenge — map one function

For add_bonus, collect evidence from:

- nm -D,
- Ghidra,
- GDB,
- process mappings.

Write one paragraph connecting all four observations.

## 24. Analysis checklist

~~~text
1. Which libraries are required?
2. Which symbols are imported?
3. Which symbols are exported?
4. Can the loader find the dependency?
5. Which path is chosen?
6. Where is the library mapped?
7. Which function address is used?
8. Does Ghidra match runtime behavior?
9. Is the ABI compatible?
10. Which evidence is static vs runtime?
~~~

## Exercises

1. Build libday25.so.
2. Build day25_app.
3. Observe runtime lookup behavior.
4. Use ldd.
5. Inspect NEEDED entries.
6. Inspect imported/exported symbols.
7. Run LD_DEBUG=libs.
8. Run LD_DEBUG=bindings.
9. Inspect relocations.
10. Open both ELF files in Ghidra.
11. Find libday25.so in GDB mappings.
12. Break at add_bonus.
13. Build the alternate local library.
14. Complete the function-mapping challenge.

## Questions

1. Static vs dynamic linking?
2. What is a shared object?
3. Why use -fPIC?
4. Why can link time succeed while runtime fails?
5. What does ldd show?
6. What is a NEEDED entry?
7. Imported vs exported symbol?
8. What does the runtime loader do?
9. What does LD_DEBUG reveal?
10. How do PLT/GOT connect to shared libraries?
11. What is lazy binding?
12. What is a relocation?
13. What is an ABI?
14. Why can one process contain mappings from multiple ELF files?

## Main takeaway

~~~text
executable needs symbol
      ↓
dynamic metadata
      ↓
loader finds shared object
      ↓
maps library
      ↓
resolves / relocates
      ↓
PLT/GOT reaches library code
      ↓
runtime behavior
~~~

Day 25 connects separate ELF files on disk to shared code inside one live process.
