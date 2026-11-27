# Gün 72 — İşaretli Sayılar ve Açık Bayt Çözümleme

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](decode.c)

Bir bayt tek başına işaretli veya işaretsiz sayı değildir. Hayalî sekiz bitlik alanı açık aritmetikle çözüyoruz; uygulamaya bağlı signed dönüşümlere güvenmiyoruz. İkinci fonksiyon iki baytı little-endian olarak birleştirir. Örnekler: 0x7f → 127, 0x80 → -128, 0xff → -1; 34 12 → 0x1234. On kontrolü tahmin et, -O0 ve -O2 derleyip debugger'da karşılaştır. Yeni 0x81 testi ekle. İki baytlık fonksiyonun girdisi geçerli dizi varsayılır; genel dosya ayrıştırıcı değildir.

## Çalıştır
```bash
cd labs/72-signedness-and-explicit-byte-decoding
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 decode.c -o decode
./decode
```
