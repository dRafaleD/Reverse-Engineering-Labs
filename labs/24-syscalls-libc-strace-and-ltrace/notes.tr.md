# Gün 24 — System Call, libc Wrapper, strace ve ltrace

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Aynı zararsız programı source code, Ghidra, `ltrace` ve `strace` üzerinden inceleyerek libc ile Linux kernel interface arasındaki ilişkiyi anlamak.

Bu gün birlikte:

- libc vs kernel,
- system call,
- library wrapper,
- dynamic linking,
- file descriptor,
- process/file activity,
- return value/error,
- static vs dynamic evidence,
- tool correlation

konularını işler.

## 1. Katmanlar

```text
C program
   ↓
libc / shared library
   ↓
system call boundary
   ↓
Linux kernel
   ↓
filesystem / process / device / network
```

Her C function system call değildir.

Örneğin:

- `strlen` library logic,
- `malloc` allocator logic,
- `printf` libc,
- `write` kernel syscall interface'ine çok yakın,
- `open` libc wrapper üzerinden `openat` gibi kernel interface'e dönüşebilir.

## 2. RE açısından neden önemli?

Static analysis:

```text
CALL printf
CALL malloc
CALL open
CALL write
```

gösterebilir.

Dynamic tracing ise runtime'da gerçekten ne olduğunu gösterir.

Sorular:

- hangi file açıldı?
- hangi path kullanıldı?
- hangi call fail oldu?
- hangi fd döndü?
- ne yazıldı?

## 3. Build

```bash
gcc -g -O0 source/syscall_trace.c -o syscall_trace
./syscall_trace
```

Program PID basar, heap allocation yapar, `day24_output.txt` oluşturur, harmless text yazar ve kapanır.

## 4. Import incele

```bash
objdump -T syscall_trace | less
```

veya:

```bash
readelf -Ws syscall_trace | less
```

Şunları ara:

- puts
- printf
- getpid
- malloc
- strlen
- strcpy
- open
- write
- close
- free

Bu static hypothesis oluşturur.

## 5. Ghidra

`main` içinde:

- output,
- PID,
- heap allocation,
- copy,
- file open,
- write,
- close,
- free

akışını bul.

Trace çalıştırmadan önce davranışı tahmin et.

## 6. System call nedir?

User space'ten kernel'e controlled request'tir.

```text
user mode
   ↓
syscall boundary
   ↓
kernel mode
```

File, process, network, memory mapping ve signal işlemlerinde karşına çıkar.

## 7. File descriptor

Yaygın:

```text
0 stdin
1 stdout
2 stderr
```

Yeni file çoğu zaman 3 gibi available integer alabilir.

```text
openat(...) = 3
write(3, ...) = ...
close(3) = 0
```

Exact fd değişebilir.

## 8. strace

```bash
strace ./syscall_trace
```

Daha temiz:

```bash
strace -e trace=openat,write,close,getpid ./syscall_trace
```

File open, write, close ve PID davranışına odaklan.

## 9. strace satırı okuma

```text
openat(AT_FDCWD, "day24_output.txt", ..., 0644) = 3
```

Şuna ayır:

- operation,
- arguments,
- path,
- flags/mode,
- return value.

```text
write(3, "...", N) = N
```

fd 3'e N byte yazıldığını gösterir.

## 10. Error

Failure çoğu zaman libc tarafında `-1` ve errno bilgisiyle görünür.

Dynamic trace error path'i görünür yapar.

Success kadar failure davranışını da incele.

## 11. ltrace

Kuruluysa:

```bash
ltrace ./syscall_trace
```

Library-call seviyesine bakar.

Environment'a göre:

- puts
- printf
- malloc
- strlen
- strcpy
- free

gibi call'lar görülebilir.

## 12. strace vs ltrace

```text
ltrace -> library call
strace -> system call
```

Örneğin:

```text
printf
  ↓ libc
write
  ↓ kernel
```

Tek high-level call birden fazla syscall üretebilir.

## 13. malloc tek syscall değildir

`malloc = brk` veya `malloc = mmap` diye ezberleme.

Allocator behavior:

- allocation size,
- current heap state,
- libc,
- previous allocation

gibi faktörlere bağlıdır.

## 14. open vs openat

Source:

```c
open(...)
```

derken strace:

```text
openat(...)
```

gösterebilir.

```text
source API != exact syscall name
```

## 15. Static vs runtime evidence

Static:

> Binary `open` import ediyor.

Runtime:

> Bu execution'da `day24_output.txt` açıldı.

Aynı claim değildir.

## 16. Ghidra ile correlate et

Ghidra'da `open` call'a git.

Backward trace yap:

- filename nereden?
- flags ne?
- mode ne?

Sonra strace ile karşılaştır.

## 17. File flags

```c
O_CREAT | O_WRONLY | O_TRUNC
```

bit flag kombinasyonudur.

Bu Day 17 bitwise/flags ile bağlantılıdır.

## 18. PID

```c
getpid()
```

çıktısını:

```bash
strace -e trace=getpid ./syscall_trace
```

ile compare et.

## 19. Loader noise

Full strace `main` başlamadan önce:

- shared library load,
- mmap,
- config read,
- runtime init

gösterebilir.

Önce filtrele; sonra full trace'e geri dön.

## 20. Mini challenge — önce tahmin et

Strace öncesi yaz:

- hangi path açılacak?
- read mi write mı?
- create olacak mı?
- ne yazılacak?
- close olacak mı?

Sonra doğrula.

## 21. Mini challenge — safe error path

Working copy'de output'u read-only bir klasöre yönlendir ve trace et.

```bash
mkdir readonly_dir
chmod 555 readonly_dir
```

İş bitince:

```bash
chmod 755 readonly_dir
rmdir readonly_dir
```

Root gibi privilege'larda behavior farklı olabilir; kendi evidence'ını yorumla.

## 22. Optimization

```bash
gcc -g -O2 source/syscall_trace.c -o syscall_trace_O2
```

Ghidra, ltrace ve strace'i compare et.

Compiler bazı library call'ları değiştirebilir/eliminate edebilir.

## 23. Malware analysis bağlantısı

İleride suspicious binary'de aynı yaklaşım:

```text
static import
   ↓
behavior hypothesis
   ↓
dynamic trace
   ↓
filesystem/process/network evidence
```

şeklinde kullanılabilir.

Bu repo şimdilik harmless target kullanır.

## 24. Worksheet

En az 6 satır:

| Observation | Tool | Layer | Meaning |
| --- | --- | --- | --- |
| open import | Ghidra/readelf | static/library | file-opening API reference |
| openat(...)=3 | strace | runtime/kernel | file opened |
| malloc | ltrace | runtime/libc | allocator called |
| write(3,...) | strace | runtime/kernel | bytes written |

## Alıştırmalar

1. Compile/run.
2. Import'ları incele.
3. Ghidra'dan behavior tahmin et.
4. Full strace çalıştır.
5. Filtered strace çalıştır.
6. fd bul.
7. open vs openat açıkla.
8. ltrace çalıştır.
9. Library/system call farkını karşılaştır.
10. File flags'i Ghidra'da bul.
11. Safe error-path challenge yap.
12. O0/O2 compare et.
13. Worksheet doldur.
14. Static/runtime observation ayır.

## Sorular

1. System call nedir?
2. libc nedir?
3. Neden her libc call syscall değildir?
4. File descriptor nedir?
5. strace vs ltrace?
6. open neden openat görünebilir?
7. malloc neden her seferinde yeni syscall üretmeyebilir?
8. Return value ne söyler?
9. Static capability vs observed behavior?
10. Full strace neden noisy?
11. Bit flags RE'de nasıl görünür?
12. Ghidra ve runtime trace neden birlikte kullanılır?
13. Optimization ltrace'i nasıl etkileyebilir?
14. Error path neden önemlidir?

## Ana çıkarım

```text
source
  ↓
libc/API
  ↓
syscall boundary
  ↓
kernel behavior
  ↓
strace / ltrace
  ↓
Ghidra correlation
```

Bu gün farklı abstraction layer'ları ayırmayı öğreniyorsun.
