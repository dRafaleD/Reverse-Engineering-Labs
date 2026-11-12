# Gün 57 — Durum Geçişleri, Yan Etkiler ve Değişiklik Noktaları

[English](notes.md) · [Türkçe](notes.tr.md) · [C örneği](transition_side_effects.c)

## 56. günden devam
56. günde hata, yeniden deneme ve kurtarma geçişlerini inceledik. Şimdi yalnızca durumun değişmesine değil, işlem sonucunda oluşan **yan etkilere (side effects)** de bakıyoruz. Örneğin bir program, işlemi tamamlandığında sayacı artırabilir. Reddedilmiş bir istek bu sayacı değiştirmemelidir.

Reverse engineering yaparken iki şeyi ayrı izleriz: **control flow (kontrol akışı)** hangi dalın çalıştığını, **data flow (veri akışı)** hangi değişkenin/bellek alanının güncellendiğini gösterir.

## Durum makinemiz
Durumlar: INIT (başlangıç), ACTIVE (çalışıyor), FAILED (hata), DONE (tamamlandı).
Olaylar: START (başlat), COMPLETE (tamamla), FAIL (hata), RETRY (yeniden dene), RESET (sıfırla).

| Mevcut durum | Olay | Yeni durum | Yan etki |
| --- | --- | --- | --- |
| INIT | START | ACTIVE | yok |
| ACTIVE | COMPLETE | DONE | completed + 1 |
| ACTIVE | FAIL | FAILED | yok |
| FAILED | RETRY | ACTIVE | retries + 1 |
| ACTIVE / FAILED / DONE | RESET | INIT | yok |

Diğer tüm eşleşmeler **reddedilir**: durum, tamamlanma sayacı ve yeniden deneme sayacı değişmez. Bu eğitim modelinde RESET sayaçları temizlemez; sayaçlar toplam işlem sayısını temsil eder.

**Commit point (değişiklik noktası):** İzin verilen dalda bir değişkenin güncellendiği kod noktasıdır. Buradaki terim veritabanı transaction'ı anlamına gelmez. Birden çok alanın güncellenmesi eşzamanlı (atomic) veya çökme durumunda güvenli değildir.

## Neyi test ediyoruz?
4 durum × 5 olay = **20 farklı kombinasyon**. C kodundaki beklenen sonuç tabloları, her durum ve olay için şunları denetler:
- İşlem kabul edildi mi?
- Yeni durum doğru mu?
- completed ve retries sayaçları yalnızca izinli durumda mı arttı?
- Reddedilen olay bütün alanları aynı mı bıraktı?

Ayrıca dokuz adımlı gerçekçi olmayan fakat anlaşılır bir test akışı vardır:
1. START → FAIL → RETRY → COMPLETE: 1 tamamlanma, 1 yeniden deneme.
2. DONE durumunda ikinci COMPLETE reddedilir: tamamlanma sayacı 1 kalır.
3. RESET, INIT durumuna döndürür; toplam sayaçlar korunur.
4. INIT durumunda RETRY reddedilir.
5. START → COMPLETE sonrası toplam tamamlanma sayısı 2 olur.

## Linux üzerinde uygula
~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 transition_side_effects.c -o transition_side_effects
./transition_side_effects
gdb -q ./transition_side_effects
objdump -d -M intel transition_side_effects | less
~~~

GDB içinde:
~~~text
(gdb) break apply
(gdb) run
(gdb) print *w
(gdb) disassemble apply
(gdb) next
(gdb) print *w
~~~

Ghidra'da apply fonksiyonunu bul. Workflow.state, Workflow.completed ve Workflow.retries alanlarına yazan assembly komutlarını tespit et. Sonra -O2 -g ile derle ve karşılaştır. Optimizasyon dalları ve komut sırasını değiştirebilir; yalnızca decompiler görünümüne güvenme. Örnek GCC ile -O0 ve -O2 seviyelerinde yerel olarak test edildi.

## Görevler
1. 4×5 tabloyu çiz; kabul edilen ve reddedilen olayları işaretle.
2. completed sayacını hangi bellek yazımı artırıyor? DONE durumunda ikinci COMPLETE neden sayacı artıramıyor?
3. Artırma işlemini kasıtlı olarak koşulun önüne taşı. Hangi assertion bozuluyor?
4. FAILED + RETRY öncesi/sonrası Workflow içeriğini GDB ile karşılaştır.
5. Bu testler neden eşzamanlılık, kalıcı kayıt veya yetkilendirme garantisi vermez?

## Sınırlar
Bu çalışma yalnızca yerel ve zararsız bir C örneğidir; haricî sisteme veya gerçek oturuma müdahale etmez. Bir durum tablosunun tamamen test edilmesi gerçek yazılımın güvenli olduğunu kanıtlamaz. Eşzamanlılık, kalıcılık, sayaç taşması ve yetkilendirme ayrıca ele alınmalıdır.
