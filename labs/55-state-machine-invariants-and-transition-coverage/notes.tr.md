# Gün 55 — Durum Makinesi Değişmezleri ve Geçiş Test Kapsamı

[English](notes.md) · [Türkçe](notes.tr.md) · [C alıştırması](transition_tests.c)

## 54. günden devam
Bir önceki lab'de programın dallarını takip ederek üç durumlu makinenin nasıl çalıştığını çıkarmıştık. Peki yalnızca **başarılı akışı** deneyerek tüm sistemi anlamış olur muyuz? Hayır. Beklenmeyen olay geldiğinde ne olduğunu da test etmeliyiz.

## Durum, olay ve değişmez (invariant)
Üç durumumuz var: `IDLE` (bekliyor), `READY` (hazır) ve `DONE` (tamamlandı). Olaylar: `START`, `CONFIRM`, `RESET`.

**İzinli geçişler:**
- `IDLE + START → READY`
- `READY + CONFIRM → DONE`
- `DONE + RESET → IDLE`

Üç durum × üç olay = **9 olası eşleşme**. Bunların üçü kabul edilir; diğer **altısı reddedilip mevcut durumda kalmalıdır**. Bu şart bizim *değişmezimizdir (invariant)*. Ayrıca fonksiyon her zaman tanımlı üç durumdan birini döndürmelidir.

Programdaki `transition()` incelenen uygulama, `expected[][]` tablosu ise **bağımsız beklenen davranış modeli** olarak tasarlandı. Aynı mantığı iki farklı şekilde ifade ederek aralarında tutarsızlık arıyoruz.

## Uygulama
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 transition_tests.c -o transition_tests
./transition_tests
gdb -q -batch -ex 'break transition' -ex run -ex 'info args' -ex 'bt' -ex quit ./transition_tests
objdump -d -M intel transition_tests | less
```
Beklenen sonuç: dokuz adet PASS, ardından `9/9 transitions verified`. GDB'de `transition` fonksiyonuna giren parametreleri incele. Daha sonra `-O2 -g` ile derleyip assembly ve Ghidra çıktılarını karşılaştır.

## Sorular
1. Dokuz hücreli bir geçiş tablosu çiz; izinli ve yasak geçişleri işaretle.
2. Sadece üç başarılı yolu denemek neden yetersizdir?
3. Yanlışlıkla `READY + RESET` geçişine izin verirsen hangi test başarısız olur?
4. Ghidra'da koşullu atlamalar nasıl görünür? Optimize derleme sonucu değişir mi?
5. Yeni bir `ERROR` durumu eklersen varsayılan reddetme yaklaşımı neden önemli?
6. Dokuz kombinasyonu kontrol etmek; thread yarışlarını, yetkisiz dış girdileri ve bellek hatalarını neden kapsamaz?

## Güvenlik sınırı
Bu çalışma gerçek kimlik doğrulama veya erişim kontrolü sistemi değil; **tersine mühendislikte davranış hipotezini test etme** alıştırmasıdır. Tam tablo kapsamı, genel güvenlik garantisi değildir.
