# Gün 61 — Nesil Kontrollü Handle ve Yetki Bitleri

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](capability_handles.c)

## 60. günden devam
Önceki gün **generation (nesil sayacı)** ile eski handle'ların, aynı bellek yuvası yeniden kullanıldığında reddedilmesini işledik. Şimdi bir adım ileri: handle doğru nesneyi gösterse bile **o işlemi yapmaya yetkili mi?**

**Capability (yetki):** bir referansın yapmasına izin verilen işlem. `CAN_READ` okuma, `CAN_WRITE` yazma yetkisini temsil eder. **Least privilege (en az ayrıcalık):** yalnızca gereken hakkı vermek.

Örnek: salt-okunur handle ile değişiklik yapılamaz. Yalnızca yazma yetkisi olan handle ile değer okunamaz. Nesnenin generation değeri artınca **eski handle'ın tüm işlemleri** reddedilir.

## Çalıştırma
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 capability_handles.c -o capability_handles
./capability_handles
gdb -q ./capability_handles
```
GDB'de `valid` fonksiyonunda durup generation ve rights alanlarını karşılaştır. Ghidra'da bit maskesini ve koşullu dallanmaları incele.

## Sorular
Doğru nesneyi göstermek neden işlem yetkisini kanıtlamaz? Eski neslin yazma izni neden kullanılmamalı? Gerçek thread veya serbest bırakılmış bellek bu modeli nasıl değiştirir?

## Sınırlar
Bu örnek gerçek sistem yetkilendirmesi veya güvenli bellek tahsisçisi değildir; nesne tüm program boyunca ayrılmış durumda tutulur.
