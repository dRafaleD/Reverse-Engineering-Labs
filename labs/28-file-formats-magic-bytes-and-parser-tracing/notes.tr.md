# Gün 28 — Dosya Formatları, Magic Byte, Endianness ve Parser İzleme

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

Bu gün önceki labların üzerine inşa edilen, yalnızca yerel ve zararsız eğitim verisi kullanan uygulamalı bir çalışmadır. Kodun her varsayımını incele; çıktıları otomatik güvenlik kanıtı sayma.

## Öğrenme hedefleri

- Magic and byte-level contracts
- Little-endian length
- Boundary checks
- Static call graph
- Dynamic tracing
- Optimization effects
- Trust and limitations
- Analysis evidence

## Veri akışı ve tehdit modeli

```text
controlled training input
        ↓
parser / evaluator / analyst
        ↓
security or evidence check
        ↓
observable result
        ↓
review assumptions and limits
```

## 1. Magic and byte-level contracts

RE28 başlığını dosya uzantısıyla değil gerçek baytlarla doğrula.

**Görev:** Bu kontrolün hangi yanlış yorum veya güven sınırı ihlalini önlediğini bir örnekle açıkla.

## 2. Little-endian length

03 00 baytları little-endian olarak 3 uzunluğunu gösterir. Parser dönüşümünü açıkla.

**Görev:** Bu kontrolün hangi yanlış yorum veya güven sınırı ihlalini önlediğini bir örnekle açıkla.

## 3. Boundary checks

Header için altı bayt gerekir; bildirilen uzunluk kalan baytlara eşit olmalı. Bozuk dosyaları dene.

**Görev:** Bu kontrolün hangi yanlış yorum veya güven sınırı ihlalini önlediğini bir örnekle açıkla.

## 4. Static call graph

Ghidra'da main, fopen, fread, memcmp, parse ve dönüş dallarını izle.

**Görev:** Bu kontrolün hangi yanlış yorum veya güven sınırı ihlalini önlediğini bir örnekle açıkla.

## 5. Dynamic tracing

GDB'de parse breakpoint'i koy, pointer/uzunluk/dönüş değerini incele; geçerli ve eksik örneği karşılaştır.

**Görev:** Bu kontrolün hangi yanlış yorum veya güven sınırı ihlalini önlediğini bir örnekle açıkla.

## 6. Optimization effects

-O2 ile derle ve decompile çıktısını karşılaştır; bazı fonksiyonlar inline olabilir.

**Görev:** Bu kontrolün hangi yanlış yorum veya güven sınırı ihlalini önlediğini bir örnekle açıkla.

## 7. Trust and limitations

Signature eşleşmesi tam doğrulama veya dosyanın güvenliği demek değildir.

**Görev:** Bu kontrolün hangi yanlış yorum veya güven sınırı ihlalini önlediğini bir örnekle açıkla.

## 8. Analysis evidence

Girdi hash'i, derleme ayarları, araç sürümü, statik ve dinamik gözlemleri kaydet.

**Görev:** Bu kontrolün hangi yanlış yorum veya güven sınırı ihlalini önlediğini bir örnekle açıkla.

## Uygulama

Yalnızca repo içindeki dosyaları kullan. Bu örnek üretim ortamına kurulacak güvenlik servisi değildir.

```bash
gcc -Wall -Wextra -g -O0 source/parser.c -o parser
python3 -c "from pathlib import Path;Path('valid.bin').write_bytes(b'RE28'+bytes([3,0])+b'ABC')"
./parser valid.bin
xxd valid.bin
gdb ./parser
```

## Test matrisi

| Senaryo | Beklenen | Neden |
|---|---|---|
| Valid training input | Kabul / eşleşme | Baseline |
| Wrong owner or header | Ret / eşleşmeme | Scope / structure |
| Truncated or missing field | Kontrollü hata | Input validation |
| Unrelated benign sample | Yanlış alarm yok | False-positive awareness |

## Alıştırmalar

1. Örneği çalıştır ve çıktıyı kaydet.
2. İlk varsayımını not et.
3. Olumlu senaryoyu doğrula.
4. Bir negatif senaryo ekle.
5. İkinci negatif senaryo ekle.
6. Bir sınır değeri dene.
7. İlgili kontrolü kaynak kodunda bul.
8. Gözlem ve çıkarımı ayrı yaz.
9. Tek bir kontrolü çalışma kopyasında değiştir.
10. Bütün senaryoları tekrar test et.
11. Sonuçların sınırlamalarını yaz.
12. Bir sayfalık kısa analiz raporu hazırla.

## Tekrar soruları

1. Bu labda hangi güven sınırı var?
2. Hangi veri kullanıcı tarafından kontrol ediliyor?
3. Olumlu testin amacı ne?
4. Negatif testin amacı ne?
5. Çıktı neyi gerçekten kanıtlıyor?
6. Çıktı neyi kanıtlamıyor?
7. En kritik input hangisi?
8. Bir sınır değeri örneği ver.
9. Fail-closed davranış nedir?
10. Bu labı prod ortamına doğrudan almak neden riskli?
11. Bir false positive senaryosu düşün.
12. Bir false negative senaryosu düşün.
13. Log hangi alanları taşımalı?
14. Hangi kişisel veri gereksiz?
15. Sürüm bilgisi niçin kaydedilir?
16. Kontrol değişince hangi testler tekrar koşulmalı?

## Mini projeler

1. Bir yeni negatif test ve beklenen çıktı ekle; çalıştırıp belgele.
2. Bir güvenlik varsayımını çalışma kopyasında boz; hangi testin yakaladığını açıkla.
3. Kaynak, hash, araç sürümü, komut, gözlem ve sınırlamalardan oluşan kısa rapor yaz.

## Özet

Kendi aracının gerçek davranışını ölç; sentetik örneği aşırı genelleme. Güvenlik kararı, izin ve adli çıkarım bağımsız doğrulama gerektirir.
