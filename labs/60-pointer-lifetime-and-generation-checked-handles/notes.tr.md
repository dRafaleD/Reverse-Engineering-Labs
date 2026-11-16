# Gün 60 — Pointer Ömrü ve Nesil Kontrollü Handle

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](generation_handles.c)

## 59. günden devam
Pointer zincirlerini inceledik. Şimdi aynı bellek adresinin **zaman içinde farklı mantıksal nesneyi** temsil edebilmesini öğreniyoruz. Yani aynı adres, aynı nesne anlamına gelmeyebilir.

**Lifetime (yaşam süresi):** nesneye güvenli erişilebilen dönem. **Stale handle:** eskimiş, artık geçerli olmayan referans. **Generation counter (nesil sayacı):** nesne yenilenince artan sürüm. **ABA benzeri durum:** adres aynı görünür ama nesne arada değiştirilmiştir.

Bu lab'deki `Slot` gerçek anlamda serbest bırakılmıyor; bellekte kalıyor. `Handle` hem adresi hem nesil numarasını tutuyor. `recycle()` çağrısından sonra eski referans aynı adrese baksa da nesil farklı olduğu için reddediliyor. Böylece **use-after-free çalıştırmadan** tehlikeli mantığı güvenli biçimde tanıyoruz.

## Çalıştırma
~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 generation_handles.c -o generation_handles
./generation_handles
gdb -q ./generation_handles
~~~
GDB'de `break read_value` koy; yenileme öncesi/sonrası `h.slot` ve `h.generation` değerlerini karşılaştır. Ghidra'da gerçek alan okumasından önce gelen nesil kontrolünü bul.

## Sorular
Aynı adres neden farklı nesneyi temsil edebilir? Eski handle neden reddedilmeli? Nesil sayacı taşarsa ne olur? Gerçek bellek serbest bırakılsaydı bu kod neden yeterli olmazdı?

## Sınırlar
Bu bir gerçek bellek yöneticisi değil; sadece nesne ömrü ve mantıksal kimlik analizi içindir. Eşzamanlı thread güvenliği garanti etmez.
