# Gün 20 — ELF Section'ları, Symbol'lar ve Stripped Binary'ler

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Linux ELF binary'sinin section'lara nasıl ayrıldığını, symbol bilgisinin reverse engineering'i nasıl kolaylaştırdığını ve binary strip edildiğinde nelerin değiştiğini anlamak.

Bu lab C source ile `.text`, `.rodata`, `.data` ve `.bss` gibi temel ELF section'larını birbirine bağlar.

## Kaynak

`source/elf_sections.c` içinde:

- executable code,
- initialized global,
- uninitialized global,
- read-only string,
- static helper function

bulunuyor.

Unstripped build:

```bash
gcc -g -O0 source/elf_sections.c -o elf_sections
```

Stripped copy:

```bash
cp elf_sections elf_sections_stripped
strip elf_sections_stripped
```

## 1. ELF nedir?

ELF, **Executable and Linkable Format** anlamına gelir.

Linux'ta yaygın olarak:

- executable,
- shared library,
- object file,
- core dump

formatlarında kullanılır.

Bugün bütün ELF specification'ını ezberlemiyoruz. Amaç binary'nin farklı amaçlara sahip düzenli bölgeler içerdiğini görmek.

## 2. Temel section'lar

```text
.text    -> executable machine code
.rodata  -> read-only constant ve string'ler
.data    -> initialized writable global/static data
.bss     -> uninitialized veya zero-initialized global/static data
```

Başka section'lar da vardır ama başlangıç için bu dördü çok önemlidir.

## 3. C variable'larını section'lara bağla

Kaynakta:

```c
int initialized_global = 42;
int uninitialized_global;
static const char message[] = "ELF section training";
```

Başlangıç beklentisi:

```text
initialized_global   -> .data
uninitialized_global -> .bss
message              -> .rodata
helper/main code     -> .text
```

Exact layout compiler/linker'a göre değişebilir fakat kategori mantığı faydalıdır.

## 4. Section'ları incele

```bash
readelf -S elf_sections
```

veya:

```bash
objdump -h elf_sections
```

Şunları bul:

- `.text`
- `.rodata`
- `.data`
- `.bss`

Adreslerini ve size değerlerini kaydet.

## 5. Symbol'ları incele

```bash
nm elf_sections
```

Şunları ara:

```text
main
helper
initialized_global
uninitialized_global
```

Debug/unstripped binary isimleri koruduğu için analiz daha kolaydır.

Alternatif:

```bash
readelf -s elf_sections
```

## 6. strip ne yapar?

`strip`, normal execution için gerekli olmayan symbol/debug bilgisini kaldırır.

Karşılaştır:

```bash
nm elf_sections
nm elf_sections_stripped
```

Stripped binary yine çalışır fakat birçok faydalı isim kaybolabilir.

Gerçek reverse engineering'de binary'lerin stripped olması sık karşılaşılan bir durumdur.

## 7. Ghidra karşılaştırması

İki binary'yi ayrı ayrı import et:

```text
elf_sections
elf_sections_stripped
```

Karşılaştır:

- function isimleri,
- global isimleri,
- strings,
- decompiler output,
- XREF'ler.

Symbol kaybolsa bile machine code ve birçok string binary içinde kalır.

## 8. String'ler strip sonrası kalabilir

```bash
strings elf_sections_stripped | grep "ELF section"
```

Symbol stripping literal string'leri otomatik olarak silmez.

Bu yüzden isimler kaybolduğunda string'ler hâlâ çok iyi başlangıç noktaları olabilir.

## 9. Section ve segment farkı

Başlangıç seviyesinde:

```text
sections -> linker/static analysis organizasyonu
segments -> loader/runtime mapping
```

`readelf -S` section'ları gösterir.

```bash
readelf -l elf_sections
```

program header/segment bilgisini gösterir.

Şimdilik detayını ezberleme; section düzeni ile runtime memory mapping'in ilişkili ama aynı kavram olmadığını bil.

## 10. Güvenlik bağlantısı

Unknown Linux binary analiz ederken section ve symbol bilgisi şunları anlamaya yardım eder:

- Executable code nerede?
- Constant/string'ler nerede?
- Writable global data nerede?
- Function isimleri korunmuş mu?
- Binary stripped mı?

Stripped binary otomatik olarak malware değildir. Normal release build'lerde de stripping çok yaygındır.

## Alıştırmalar

1. Unstripped binary'yi compile et.
2. `readelf -S` ile section'ları listele.
3. Bu labdaki dört temel section'ı bul.
4. `nm` ile symbol'ları listele.
5. Stripped copy oluştur.
6. Strip öncesi/sonrası `nm` output'unu karşılaştır.
7. İki binary'yi Ghidra'da açıp function isimlerini karşılaştır.
8. Stripped binary içinde `"ELF section training"` string'ini bul.
9. String XREF'ini onu kullanan code'a kadar takip et.

## Sorular

1. `.text` ne amaçla kullanılır?
2. Initialized global neden genellikle `.data` içinde olur?
3. Uninitialized global neden genellikle `.bss` içinde olur?
4. `.rodata` içinde ne tür veriler görürüz?
5. Stripping neyi kaldırır?
6. Stripped binary çalışmaya devam eder mi?
7. Symbol'lar silindiğinde string'ler neden faydalı kalır?
8. ELF section ve runtime segment birebir aynı kavram mıdır?

## Ana çıkarım

```text
ELF
 ├─ .text   -> code
 ├─ .rodata -> constants/strings
 ├─ .data   -> initialized writable globals
 └─ .bss    -> zero/uninitialized globals
```

Sonra symbol durumunu düşün:

```text
symbols present -> navigation daha kolay
symbols stripped -> strings + XREF + control flow + behavior daha önemli
```
