# Gün 69 — Uygulama Günü: İkili Kayıt Ayrıştırma ve Sınır Kontrolü

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](record_parser.c)

Bugünkü uygulamada hayalî bir ikili kayıt biçimini güvenli şekilde inceliyoruz. Altı baytlık başlık: LB magic, sürüm 1, tür 1/2 ve little-endian iki bayt uzunluk. En fazla sekiz bayt veri kabul edilir. Başlık okunmadan önce boyut, ardından gerçek veri uzunluğu kontrol edilir.

| Hex örneği | Beklenen | Neden |
|---|---|---|
| 4c 42 01 01 00 00 | OK | Boş veri |
| 4c 42 01 02 01 00 2a | OK | Bir bayt veri |
| 4c 42 01 01 00 01 | BAD_LENGTH | Little-endian 256 |
| 4c 42 01 01 02 00 2a | TRUNCATED | Eksik veri |
| 4c 42 01 01 00 00 2a | TRAILING | Fazladan bayt |

**Uygulamalar:** 1. 14 senaryonun çıktısını önceden tahmin et. 2. GDB ile parse fonksiyonunda başlık baytlarını incele. 3. -O2 derleyip Ghidra'da sınır kontrollerini karşılaştır. 4. İki baytlık geçerli örnek ekle. 5. Başarısız ayrıştırmada çıktı değişkeninin değişmediğini doğrula.

Beklenen son satır: 14/14 cases passed. NDEBUG kullanmadan derle. Gerçek dosya veya canlı sistem kullanılmaz.

## Çalıştır
```bash
cd labs/69-binary-record-parsing-practice
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 record_parser.c -o record_parser
./record_parser
```
