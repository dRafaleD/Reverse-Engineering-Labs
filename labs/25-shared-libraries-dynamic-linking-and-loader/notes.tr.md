# Gün 25 — Shared Library, Dynamic Linking, Loader Resolution ve LD_DEBUG

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Linux executable'ın runtime'da shared library içindeki code'u nasıl bulup kullandığını anlamak.

Bu gün shared object, dynamic linking, imported/exported symbol, runtime loader, search path, PLT/GOT, ldd, readelf, nm, LD_DEBUG, relocation, ABI ve Ghidra + GDB correlation konularını birlikte işler.

## 1. Static vs dynamic linking

~~~text
executable
   ↓ needs symbol
shared library
   ↓
runtime loader resolves
~~~

Dynamic executable dependency code'u runtime'da shared object'ten kullanır.

## 2. Shared object

Bu lab libday25.so oluşturur.

Library:

- add_bonus
- print_banner

function'larını export eder.

## 3. Library build

~~~bash
gcc -fPIC -shared source/libday25.c -o libday25.so
~~~

-fPIC position-independent code üretir.

-shared shared object üretir.

## 4. Executable build

~~~bash
gcc -g -O0 source/main.c -L. -lday25 -o day25_app
~~~

Link-time search path ile runtime search path aynı şey değildir.

## 5. Runtime lookup

~~~bash
./day25_app
~~~

Library bulunamazsa local lab için:

~~~bash
LD_LIBRARY_PATH=. ./day25_app
~~~

Beklenen result 17'dir.

## 6. ldd

~~~bash
ldd ./day25_app
LD_LIBRARY_PATH=. ldd ./day25_app
~~~

Dependency ve resolved path'i incele.

## 7. Dynamic section

~~~bash
readelf -d day25_app
~~~

NEEDED entry'lerini bul.

libday25.so dependency'sini gör.

## 8. Export/import symbol

Library:

~~~bash
nm -D libday25.so
~~~

Executable:

~~~bash
nm -D day25_app
~~~

~~~text
app needs symbol
library provides symbol
~~~

mantığını gör.

## 9. Runtime loader

Loader:

- library bulur,
- memory'ye map eder,
- relocation işler,
- symbol resolve eder,
- programı başlatır.

## 10. LD_DEBUG

~~~bash
LD_DEBUG=libs LD_LIBRARY_PATH=. ./day25_app
LD_DEBUG=bindings LD_LIBRARY_PATH=. ./day25_app
~~~

Search ve binding kararlarını incele.

## 11. PLT/GOT

~~~text
main
  ↓
PLT
  ↓
GOT / resolver
  ↓
shared-library code
~~~

Day 18 ile bağla.

## 12. Lazy binding

Bazı symbol'lar first call sırasında resolve edilebilir.

Exact behavior toolchain/settings'e bağlıdır.

## 13. Relocation

~~~bash
readelf -r day25_app
readelf -r libday25.so
~~~

Mantık:

~~~text
build-time reference
      ↓
runtime address
      ↓
loader adjustment
~~~

## 14. GDB mappings

~~~text
set environment LD_LIBRARY_PATH .
break main
run
info proc mappings
~~~

App, library, libc ve loader mapping'lerini bul.

## 15. Library function breakpoint

~~~text
break add_bonus
continue
info args
backtrace
x/i $rip
~~~

Başka ELF object içindeki code'da durmuş olursun.

## 16. Multiple ELF, one process

~~~text
disk:
day25_app
libday25.so

runtime:
one process
  ├─ app
  ├─ libday25.so
  ├─ libc
  └─ loader
~~~

## 17. ABI

Dynamic linking symbol name, calling convention, parameter ve return expectation'larının uyumuna bağlıdır.

Uyumsuzluk ABI problemidir.

## 18. Alternate library challenge

Kendi local working copy'nde add_bonus'u +100 yap.

System library değiştirme.

Aynı executable'ın farklı compatible library implementation ile behavior'unu karşılaştır.

## 19. Security relevance

Analizde sor:

- hangi library loaded?
- hangi path'ten?
- hangi symbol resolve?
- runtime static expectation ile uyuşuyor mu?

Odak provenance ve resolution'dır.

## 20. Mini challenge

Dependency fail nedenini kendin bul.

ldd, readelf ve LD_DEBUG sonuçlarını birleştir.

## 21. Checklist

~~~text
1. Required library?
2. Imported symbol?
3. Exported symbol?
4. Loader dependency'yi buluyor mu?
5. Hangi path?
6. Nerede mapped?
7. Hangi symbol address?
8. Ghidra/runtime uyumlu mu?
9. ABI compatible mı?
10. Evidence static mi runtime mı?
~~~

## Alıştırmalar

1. Library build.
2. App build.
3. Runtime lookup gözlemle.
4. ldd.
5. NEEDED inspect.
6. Symbol inspect.
7. LD_DEBUG libs.
8. LD_DEBUG bindings.
9. Relocation inspect.
10. Ghidra import.
11. GDB mapping.
12. add_bonus breakpoint.
13. Alternate library.
14. Function mapping challenge.

## Sorular

1. Static vs dynamic linking?
2. Shared object nedir?
3. -fPIC neden?
4. Build success olup runtime neden fail eder?
5. ldd ne gösterir?
6. NEEDED nedir?
7. Import/export symbol farkı?
8. Loader ne yapar?
9. LD_DEBUG ne gösterir?
10. PLT/GOT nasıl bağlanır?
11. Lazy binding nedir?
12. Relocation nedir?
13. ABI nedir?
14. Tek process neden birden fazla ELF map eder?

## Ana çıkarım

~~~text
executable
  ↓ dependency
loader
  ↓ map + resolve
PLT/GOT
  ↓
shared library function
~~~

Day 25 ayrı ELF dosyalarının runtime'da tek process içinde nasıl birleştiğini gösterir.
