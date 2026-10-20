# Gün 34 — ELF Sembol Görünürlüğü ve Paylaşılan Kütüphane ABI

[English](notes.md) | [Türkçe](notes.tr.md)

## Öğrenme amacı
ELF Sembol Görünürlüğü ve Paylaşılan Kütüphane ABI konusunu zararsız ve tekrarlanabilir bir örnek üzerinde çalışıyoruz. Kaydedilen çıktı ile bundan çıkarılan sonucu birbirinden ayırıyoruz.

## Kavramsal bağlantılar
1. **Girdi ve güven sınırı:** Hangi verinin kim tarafından kontrol edildiğini ve güven varsayımlarını belirle.
2. **Mekanizma:** Programın hangi kontrolü hangi sırayla uyguladığını kaynak koddan takip et.
3. **Beklenen çıktı:** Kodu çalıştırmadan önce sonucu tahmin et.
4. **Olumsuz senaryo:** Geçersiz bir örneğin neden farklı davranması gerektiğini açıkla.
5. **Tekrar üretilebilirlik:** Araç sürümlerini, komutları, girdileri ve çıktılarını kaydet.
6. **Sınırlar:** Yerel eğitim örneğinin gerçek üretim ortamı güvenliğini tek başına kanıtlamadığını unutma.

## Uygulama
Lab klasöründe çalıştır:

```bash
gcc -g -O0 -fvisibility=hidden visibility.c -o visibility_demo
```

```bash
./visibility_demo
```

```bash
readelf -Ws visibility_demo | grep add
```

```bash
objdump -d visibility_demo | less
```

## İnceleme adımları
1. Kaynak kodu oku ve önceden beklediğin sonucu yaz.
2. Programı çalıştır, konsol çıktısını kaydet.
3. Her güven sınırı için kaynak kodda ilgili satırı bul.
4. Zararsız bir girdiyi değiştirip sonucu karşılaştır.
5. Zayıf bir uygulamayı yakalayabilecek negatif regresyon testi ekle.
6. Yanlış pozitif veya yanlış negatif ihtimalini açıkla.
7. Kanıt, belirsizlik ve sonraki doğrulama adımını içeren kısa rapor hazırla.

## Mini görev
Bir güvenlik özelliğini otomatik assertion ile doğrula. Ardından bu özellik bozulursa başarısız olması gereken ikinci bir test tasarla.

## Tekrar soruları
1. Çalışmanın temel kavramı nedir?
2. Hangi veri veya dosya güvenilmeyen girdidir?
3. Mevcut test tam olarak neyi kanıtlar?
4. Test neyi kanıtlamaz?
5. Çalışma ortamındaki hangi değişiklik sonucu etkiler?
6. Negatif test neden gereklidir?
7. Sonucu destekleyecek ikinci kanıt kaynağı nedir?
8. Profesyonel raporda hangi sınırlar belirtilmelidir?

## Güvenlik
Yalnızca sana ait veya açıkça izin verilen sistemlerde çalış. Tek bir işareti kesin kötü amaçlı faaliyet kanıtı olarak sunma.