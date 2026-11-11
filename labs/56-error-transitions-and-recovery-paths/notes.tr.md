# Gün 56 — Derlenmiş Programlarda Hata Geçişleri ve Kurtarma Yolları

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](recovery.c)

## Önceki günden devam
55. günde üç durum ve üç olayın tüm eşleşmelerini test ettik. Şimdi **hata**, **yeniden deneme** ve **sıfırlama** ekliyoruz. Gerçek programlarda en tehlikeli mantık hatalarından biri, hatadan kurtarma yolunun yanlışlıkla bir yetki veya durum atlama yoluna dönüşmesidir.

Bu zararsız örneğin durumları: INIT (başlangıç), ACTIVE (etkin), FAILED (hata). Olaylar: START, FAIL, RETRY, RESET.

İzinli geçişler:
- INIT + START → ACTIVE
- ACTIVE + FAIL → FAILED
- FAILED + RETRY → ACTIVE
- ACTIVE + RESET → INIT
- FAILED + RESET → INIT

Diğer eşleşmeler **reddedilir ve mevcut durum korunur**. Özellikle INIT + RETRY, ACTIVE durumuna geçiremez.

## Kavramlar
**State (durum):** programın içinde bulunduğu aşama. Bu, kullanıcının gerçekten yetkili olduğunu kanıtlamaz.

**Transition guard (geçiş koşulu):** belirli bir olayın hangi durumda kabul edileceğini belirler.

**Invariant (değişmez kural):** her zaman sağlanması beklenen özellik. Örneğin reddedilen işlem mevcut durumu değiştirmemelidir.

**Recovery path (kurtarma yolu):** FAILED durumundan çıkmak için açıkça tanımlanan geçiştir. Kurtarma işlemi, başlangıçtan kontrolsüz şekilde ilerlemeyi sağlamamalıdır.

Program **3 × 4 = 12** durum/olay çiftini bağımsız bir beklenen-sonuç tablosuyla karşılaştırır. Sonra hata ve tekrar deneme akışını çalıştırır.

## Uygulama
~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 recovery.c -o recovery
./recovery
gdb -q -batch -ex 'break step' -ex run -ex 'info args' -ex 'bt' -ex quit ./recovery
objdump -d -M intel recovery | less
~~~

Ghidra'da step fonksiyonuna bak; dalları ve hangi olayın hangi durumu döndürdüğünü bul. -O2 -g ile tekrar derleyerek assembly farklarını incele.

## Görevler
1. Üç durumu ve izinli beş geçişi diyagramda göster.
2. INIT + RETRY neden reddedilmeli?
3. FAILED + RETRY kodunu bilerek boz; hangi test hata veriyor?
4. GDB'de gözlemlediğin bir çağrıyı beklenen sonuç tablosuyla karşılaştır.
5. Bir web servisi için eşzamanlı istek, veritabanı ve kullanıcı yetkisi neden ayrıca ele alınmalı?

## Sınırlar
Bu bir **durum makinesi alıştırmasıdır**; gerçek bir oturum veya kimlik doğrulama sistemi değildir. Tüm tabloyu test etmek, gerçek uygulamanın güvenli olduğunu kanıtlamaz.
