# Gün 58 — Pointer Aliasing ve Yerinde Veri Değişikliklerini İzleme

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](aliasing.c)

## 57. günden devam
Önceki gün durum ve sayaç değişikliklerini izledik. **Pointer aliasing**, iki farklı pointer'ın aynı bellek nesnesini göstermesidir. Parametre adları `a` ve `b` olsa da adresleri aynı olabilir.

`add_events` iki farklı sayacı artırır; aynı sayaç iki kez verilirse yalnızca **bir kez** artırır. `NULL`, sıfır artış veya unsigned taşması varsa hiçbir değişiklik yapılmaz. Bu kurala **validate before mutate** (değiştirmeden önce doğrula) denir.

## Uygulama
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 aliasing.c -o aliasing
./aliasing
gdb -q ./aliasing
```
GDB'de `break add_events`, `run`, `print a`, `print b`, `print a == b` komutlarını kullan. Ghidra'da aynı nesneye ikinci yazmayı engelleyen koşulu bul. `-O2` ile karşılaştır.

## Sorular
İki parametre neden tek nesneyi gösterebilir? Taşma neden ilk yazmadan önce kontrol edilir? İki thread aynı nesneyi değiştirirse ne olur?

## Sınırlar
Bu C uygulaması atomik işlem ve çoklu thread güvenliği sağlamaz; yalnızca yerel eğitim örneğidir.
