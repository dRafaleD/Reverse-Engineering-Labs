# Gün 35 — ELF Sertleştirme: PIE, NX, RELRO ve Stack Canary

[English](notes.md) | [Türkçe](notes.tr.md)

## Amaç
ELF Sertleştirme: PIE, NX, RELRO ve Stack Canary konusunda yerel, zararsız ve tekrarlanabilir bir lab. Her sonuçta gözlem, çıkarım ve belirsizlikleri ayrı kaydedin.

## Temel kavramlar
- Önce mevcut durumu ve beklenen davranışı belirle.
- Girdi doğrulamasını ve güven sınırını belirle.
- Yalnızca bir değişkeni değiştirip sonucu karşılaştır.
- Negatif test, yanlış pozitif ve yanlış negatifleri değerlendir.
- Sürüm, komut, sonuç ve sınırlamaları yaz.

## Yerel uygulama
```bash
gcc -g -O0 -fstack-protector-strong -fPIE -pie -Wl,-z,relro,-z,now hardening.c -o hardened
```

```bash
readelf -hW hardened
```

```bash
readelf -lW hardened
```

```bash
readelf -dW hardened
```

## Alıştırmalar
1. Koddan beklenen sonucu tahmin et.
2. Komutları çalıştırıp çıktıyı kaydet.
3. Zararsız bir girdiyi değiştir.
4. Pozitif ve negatif test karşılaştırması yap.
5. Bağımsız doğrulama yöntemi öner.
6. Çıkarımlar ve belirsizlikler hakkında kısa rapor yaz.

## Mini görev
Bir güvenlik özelliği için otomatik regresyon testi yaz ve hangi hatayı yakaladığını açıkla.

## Kontrol soruları
1. Bu labda güven sınırı nerede?
2. Hangi bulgu gözlem, hangisi varsayım?
3. Negatif test neden önemli?
4. Çevre veya sürüm değişikliği sonuçları nasıl etkiler?
5. Gerçek hayatta hangi ek önlemler gerekir?

## Etik
Yalnızca kendine ait veya açık izinli sistemlerde çalış.
