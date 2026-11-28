# Gün 73 — Sabit Noktalı Sayı Çözümleme ve Aralık Kontrolü

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](fixed_point.c)

72. günde işaretli baytları açık biçimde çözmüştük. Bugün hayalî iki bayt little-endian sıcaklık alanını onda bir derece olarak yorumluyoruz. İşaret dönüşümünden sonra -400 ile 1250 aralığı kontrol edilir. FA 00 = 25,0 derece; FF FF = -0,1; 70 FE = -40,0. Hatalı alan çıktıyı değiştirmez. Dokuz değer ve üç hatalı argüman senaryosunu tahmin et. GDB'de raw ve value değerlerini incele; 01 00 örneğini ekle. Gerçek sensör protokolü değildir.

## Çalıştır
```bash
cd labs/73-fixed-point-decoding-and-range-validation
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 fixed_point.c -o fixed_point
./fixed_point
```
