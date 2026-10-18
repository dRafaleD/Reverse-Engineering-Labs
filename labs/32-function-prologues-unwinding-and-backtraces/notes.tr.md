# Gün 32 — Fonksiyon Girişleri, Stack Unwinding ve Backtrace Güvenilirliği

[English](notes.md) · [Türkçe](notes.tr.md)

## Öğrenme hedefleri
**Fonksiyon Girişleri, Stack Unwinding ve Backtrace Güvenilirliği** konusunu kontrollü bir örnekle öğreniyoruz. Amaç yalnızca çıktıyı üretmek değil, güven sınırlarını, çevreye bağlı farklılıkları ve analiz sonuçlarının güvenilirliğini yorumlamak.

## Temel kavramlar
1. **Başlangıç ölçümü:** Değişiklik yapmadan önce bilinen davranışı gözlemle.
2. **Güven sınırları:** Kullanıcı girdisini, aracın ölçtüğü veriyi ve kişisel çıkarımı ayrı tut.
3. **Çalışma ortamı:** İşletim sistemi, sürüm, yapılandırma ve çalışma koşullarının etkisini belirle.
4. **Regresyon:** İyileştirmeden sonra pozitif ve negatif örnekleri yeniden çalıştır.
5. **Delil yorumu:** Tek bir gözlemin neyi kanıtladığını ve neyi kanıtlamadığını açıkça yaz.

## Uygulama
Bu klasördeki örnek tamamen yereldir. Komutları lab klasöründen çalıştır:

```bash
gcc -g -O0 -fno-omit-frame-pointer unwind_demo.c -o frames
```

```bash
gcc -g -O2 -fomit-frame-pointer unwind_demo.c -o optimized
```

```bash
gdb -q ./frames
```

```bash
readelf --debug-dump=frames frames
```

**Beklenen davranış:** Compare backtrace and disassembly with and without frame pointers.

## Adım adım çalışma
1. Örnek kodu okuyup çalıştırmadan önce sonucu tahmin et.
2. İki kez çalıştır; sabit kalan ve değişebilen alanları kaydet.
3. Zararsız bir girdiyi değiştir, hipotez yaz ve sonucu karşılaştır.
4. Negatif test ekleyerek beklenmeyen davranışların nasıl ele alındığını incele.
5. Bir yanlış pozitif ve bir yanlış negatif ihtimali tanımla.
6. Sonucu doğrulamak için ikinci bağımsız bilgi kaynağı öner.
7. Yöntem, bulgu, belirsizlik ve sonraki adımı içeren kısa rapor hazırla.

## Mini görev
İlk güvenlik özelliğini doğrulayan küçük bir otomatik test yaz. Test geçse bile bütün sistemin güvenli olduğunu neden söyleyemeyeceğini açıkla.

## Tekrar soruları
1. Bu örnekte güven sınırı nerede?
2. Hangi sonuç doğrudan gözlem, hangisi yorum?
3. İşletim sistemi ve sürüm davranışı nasıl etkileyebilir?
4. Beklenmeyen sonucun olası nedenleri neler?
5. Bağımsız doğrulama nasıl yapılır?
6. Gerçek dünyada hangi korumalar gerekir?
7. Tek gösterge neden kesin kanıt değildir?
8. Regresyon testinde hangi durumlar bulunmalı?

## Güvenlik
Yalnızca yerel, sentetik ve izinli örneklerde çalış. Bulguları gerçek saldırı kanıtı gibi gösterme.
