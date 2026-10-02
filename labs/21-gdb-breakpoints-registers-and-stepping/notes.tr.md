# Gün 21 — GDB Temelleri: Breakpoint, Register ve Adım Adım Çalışma

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Sadece static analysis yapmaktan çıkıp GDB ile temel dynamic analysis'e geçmek.

Önceki lab'lerde çoğunlukla şu soruyu sorduk:

> Bu binary ne yapıyor gibi görünüyor?

Dynamic analysis buna yeni bir soru ekler:

> Program çalışırken gerçekte ne yapıyor?

Bu lab:

- static vs dynamic analysis
- debug symbol
- breakpoint
- stepping
- stack frame
- register inceleme
- local variable
- runtime memory
- source, assembly ve runtime state ilişkisi
- Ghidra ile GDB gözlemlerini karşılaştırma

konularını içerir.

Kullandığımız program tamamen zararsız local training binary'dir.

## 1. Static ve dynamic analysis

Static analysis programı çalıştırmadan inceler.

Örnek:

- Ghidra decompiler
- `objdump`
- `readelf`
- `strings`

Dynamic analysis program çalışırken gözlem yapar.

Örnek:

- function'da durmak
- register incelemek
- local variable görmek
- instruction adımlamak
- runtime memory incelemek

Biri diğerinin yerine geçmez.

```text
static hypothesis
      ↓
interesting location seç
      ↓
debugger altında çalıştır
      ↓
runtime state gözlemle
      ↓
hypothesis'i doğrula veya düzelt
```

## 2. Training source

Source üç function içeriyor:

```text
main
 ├─ transform
 └─ classify
```

`transform()` basit arithmetic yapar.

`classify()` bir branch içerir.

Böylece function call, local variable ve conditional path'i GDB içinde gözlemleyebiliriz.

## 3. Debug için compile et

```bash
gcc -g -O0 -fno-omit-frame-pointer source/debug_target.c -o debug_target
```

- `-g`: debug information ekler.
- `-O0`: optimization'ı azaltır; source ile machine code ilişkisini başlangıçta daha rahat görürüz.
- `-fno-omit-frame-pointer`: frame pointer davranışını bu lab için daha kolay incelenebilir tutar.

Önce normal çalıştır:

```bash
./debug_target
```

Beklenen:

```text
input=10 result=23 category=1
```

## 4. GDB başlat

```bash
gdb ./debug_target
```

GDB içinde:

```gdb
break main
run
```

Program hemen bitmek yerine breakpoint'te durur.

Breakpoint, execution'ı kontrollü şekilde durdurduğun noktadır.

## 5. Symbol ile breakpoint

Bu build symbol içerdiği için:

```gdb
break transform
break classify
info breakpoints
run
```

kullanabilirsin.

Execution function'a geldiğinde GDB durur.

Bu, symbol'ların debugging/reverse engineering'i neden kolaylaştırdığını gösterir. Day 20'de stripped binary'de bu kolaylığın bir kısmının nasıl kaybolduğunu görmüştük.

## 6. continue, next ve step

```gdb
continue
next
step
```

### continue

Bir sonraki breakpoint, signal veya program exit'e kadar devam eder.

### next

Mevcut source line'ı çalıştırır ve genellikle called function'ın **üzerinden geçer**.

### step

Mevcut source line'ı çalıştırır ve debug information uygunsa called function'ın **içine girer**.

Deney:

1. `main`'de dur.
2. `transform()` call'a kadar `next`.
3. Yeniden başlat.
4. Aynı yerde `step` kullan.
5. Farkı gözlemle.

## 7. Source-level variable inceleme

```gdb
info locals
print input
print result
```

Execution henüz assignment'a gelmediyse variable anlamlı initialized değere sahip olmayabilir.

Runtime value yorumlarken önce:

> Şu anda programın neresinde duruyorum?

sorusunu sor.

## 8. Function argument inceleme

`transform` içinde dur:

```gdb
break transform
run
info args
print value
```

İlk call'da `value`, `main` tarafından gönderilen argument ile eşleşmelidir.

Bu önceki calling convention lab'leriyle doğrudan bağlantılıdır.

## 9. Register'lar

x86-64 Linux:

```gdb
info registers
```

Önemli register'lardan bazıları:

- `rax`
- `rbx`
- `rcx`
- `rdx`
- `rsi`
- `rdi`
- `rsp`
- `rbp`
- `rip`

Başlangıç için:

```text
RIP -> current/next instruction location
RSP -> stack pointer
RBP -> bu unoptimized build'de frame reference olarak kullanışlı
RAX -> çoğunlukla return value için kullanılır
```

System V AMD64 calling convention'da integer/pointer argument'lar genellikle `RDI`, `RSI`, `RDX`, `RCX`, `R8`, `R9` sırasıyla başlar.

`transform(int value)` içinde:

```gdb
print value
p/x $rdi
```

karşılaştır.

Optimized binary'de source ile register ilişkisi her zaman bu kadar temiz görünmeyebilir.

## 10. Current instruction

```gdb
x/i $rip
```

Function assembly:

```gdb
disassemble transform
```

veya source bilgisi uygunsa:

```gdb
disassemble /m transform
```

Bağlantı:

```text
C source
   ↓
Ghidra decompilation
   ↓
assembly
   ↓
runtime RIP
```

## 11. Instruction stepping

```gdb
stepi
nexti
```

- `stepi`: bir machine instruction çalıştırır.
- `nexti`: mümkün olduğunda call'ın içine girmeden bir instruction ilerler.

Örnek:

```gdb
x/i $rip
stepi
x/i $rip
```

RIP'in nasıl ilerlediğini gözlemle.

## 12. Stack frame

`transform` içinde:

```gdb
backtrace
frame
info frame
```

Backtrace kavramsal olarak:

```text
transform
main
```

gösterebilir.

Day 5 ve Day 16'daki call stack fikrini artık runtime'da görüyorsun.

## 13. Stack memory

```gdb
x/8gx $rsp
```

Anlamı:

```text
x -> memory examine
8 -> sekiz unit
g -> 8-byte giant word
x -> hexadecimal gösterim
```

Her değerin hemen anlamlı olmasını bekleme. Stack; compiler davranışına göre saved state, return-related information, local, padding ve başka runtime data içerebilir.

## 14. Local variable address

Function içinde:

```gdb
print &doubled
print &adjusted
x/wd &doubled
```

Bağlantı:

```text
C variable
   ↓
runtime address
   ↓
memory
```

Exact address sistem ve execution arasında değişebilir.

## 15. Branch'i gözlemle

```gdb
break classify
run
print value
disassemble classify
```

Sonra comparison ve conditional jump'a doğru `stepi` ile ilerle.

C'deki:

```c
if (value > 20)
```

machine code'da comparison + control-flow decision'a dönüşür.

Bu Day 6 ile doğrudan bağlantıdır.

## 16. Diğer branch'i test et

Bu başlangıç labında runtime patch yapmak yerine source'taki:

```c
int input = 10;
```

değerini:

```c
int input = 5;
```

yap, tekrar compile et ve debug et.

`classify()` içindeki diğer path'i karşılaştır.

## 17. Ghidra + GDB workflow

`debug_target` binary'sini Ghidra'ya import et.

Bul:

- `main`
- `transform`
- `classify`

GDB çalıştırmadan önce tahmin et:

1. Hangi function `10` alacak?
2. `transform` ne döndürecek?
3. `classify` hangi branch'i alacak?

Sonra dynamic analysis ile doğrula.

```text
oku -> tahmin et -> gözlemle -> açıkla
```

## 18. Optimization ne değiştirir?

```bash
gcc -g -O2 source/debug_target.c -o debug_target_O2
```

`-O0` ile karşılaştır.

Optimization:

- function inline edebilir,
- variable'ı kaldırabilir,
- operation sırasını değiştirebilir,
- değeri yalnızca register'da tutabilir,
- source stepping'i daha az sezgisel hale getirebilir.

GDB “optimized out” diyorsa bu otomatik olarak debugger hatası değildir.

## 19. Stripped binary ne değiştirir?

```bash
cp debug_target debug_target_stripped
strip debug_target_stripped
gdb ./debug_target_stripped
```

Original build ile function-name visibility'yi karşılaştır.

Machine code hâlâ vardır fakat kullanışlı name/debug detail'lerin çoğu kaybolabilir.

Bu Day 20'nin doğrudan devamıdır.

## 20. ASLR farkındalığı

Modern sistemler process memory layout'un bazı bölümlerini randomize eder.

Bu nedenle absolute runtime address'ler execution'lar arasında değişebilir.

Şimdilik:

- debug build'de symbol-based breakpoint kullan,
- address ezberlemek yerine relationship'e odaklan,
- address'in değişebilmesini normal kabul et.

ASLR ve process memory layout ileride daha derin incelenebilir.

## Güvenli workflow

```text
harmless target compile et
       ↓
static inspect et
       ↓
breakpoint koy
       ↓
GDB altında çalıştır
       ↓
args / locals / registers incele
       ↓
instruction step et
       ↓
Ghidra ile karşılaştır
       ↓
gözlemleri yaz
```

## Alıştırmalar

1. `-O0` debug build oluştur.
2. `main`'de breakpoint koy.
3. `transform` call çevresinde hem `next` hem `step` dene.
4. `transform` içinde `value` incele.
5. x86-64 Linux'ta `value` ile `$rdi` karşılaştır.
6. `x/i $rip` ile current instruction göster.
7. Birkaç instruction `stepi` ile ilerle.
8. `transform` içindeyken `backtrace` çalıştır.
9. `$rsp` çevresindeki küçük memory bölgesini incele.
10. `classify` içinde branch'i gözlemle.
11. Source input'u değiştirip diğer path'i incele.
12. `-O0`, `-O2` ve stripped build'leri karşılaştır.

## Sorular

1. Static ve dynamic analysis farkı nedir?
2. Breakpoint nedir?
3. `next` ve `step` farkı nedir?
4. `nexti` ve `stepi` farkı nedir?
5. `RIP` neyi gösterir?
6. `RSP` neyi gösterir?
7. Symbol debugger'da neden faydalıdır?
8. Variable neden “optimized out” olabilir?
9. Runtime address neden run'lar arasında değişebilir?
10. Ghidra + GDB birlikte kullanmak neden daha güçlüdür?

## Ana çıkarım

```text
static analysis
      ↓
hypothesis
      ↓
breakpoint
      ↓
runtime state
      ↓
register + memory + control flow
      ↓
doğrulanmış anlayış
```

Day 21, compiled programı sadece okumaktan onu çalışırken gözlemlemeye geçiştir.
