# Gün 52 — Binary Dizgeleri ve Çapraz Referanslar

[English](notes.md) · [Türkçe](notes.tr.md) · [Pratik](xrefs.c)

## Hedef

Bu çalışma yalnızca izinli ve zararsız yerel örnekler üzerinden **Binary Dizgeleri ve Çapraz Referanslar** konusunu ele alır. Çıktıların doğrudan gözlem mi, yorum mu olduğunu ayırmayı hedefler.

## Temel bilgiler

1. Sabit metinler çoğunlukla salt okunur veri bölümlerinde bulunur; disassembler bunları kullanan komutlara çapraz referans gösterebilir.

2. Xref, analizde yol gösterir; ilgili kod dalının gerçekten çalıştığını tek başına kanıtlamaz.

3. Optimizasyon metinleri birleştirebilir, kaldırabilir veya kodu yeniden düzenleyebilir; sonuç derleme bayraklarına bağlıdır.

## Uygulama

Önce dosyayı oku, sonucu tahmin et, ardından çalıştır:

```bash
gcc -std=c11 -g -O0 xrefs.c -o xrefs && ./xrefs
```

Compare `strings -a xrefs`, `objdump -d xrefs` and Ghidra string XREFs. Record architecture and flags.

## Görevler

1. Beklenen ve gözlenen sonuçları karşılaştır.
2. Bir pozitif ve bir negatif örneği belgele.
3. Tek bir göstergeyle kesin sonuç çıkarmanın riskini açıkla.
4. Kodu zararsız bir sınır değerle değiştir ve testi tekrarla.

## Bilgi kontrolü

- Girdi hangi güven sınırını geçiyor?
- Hangi veri eksik veya yanlış yorumlanabilir?
- Gerçek sistemde hangi bağımsız doğrulama gerekir?

## Güvenlik ve sınırlar

Gerçek zararlı dosyalar, saldırı hedefleri veya üretim sistemleri kullanılmaz. Bu küçük eğitim örneği saha performansı garantilemez.
