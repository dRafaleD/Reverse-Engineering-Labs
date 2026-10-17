# Gün 31 — Thread, Thread-Local Storage ve GDB ile Thread İzleme

[English](notes.md) · [Türkçe](notes.tr.md)

## Hedef
Thread, Thread-Local Storage ve GDB ile Thread İzleme konusunu tamamen yerel ve zararsız örneklerle öğrenmek. Gözlemlenen bulguyu yorumdan ayırmak ve kesin olmayan sonuçları abartmamak.

## Temel kavramlar
1. **pthread_create and pthread_join:** Kavramın amacını, güven sınırını, olası hatalarını ve hangi durumda kullanılacağını açıklayın.
2. **thread-local storage:** Kavramın amacını, güven sınırını, olası hatalarını ve hangi durumda kullanılacağını açıklayın.
3. **shared versus thread-local memory:** Kavramın amacını, güven sınırını, olası hatalarını ve hangi durumda kullanılacağını açıklayın.
4. **synchronization and data races:** Kavramın amacını, güven sınırını, olası hatalarını ve hangi durumda kullanılacağını açıklayın.
5. **GDB info threads:** Kavramın amacını, güven sınırını, olası hatalarını ve hangi durumda kullanılacağını açıklayın.
6. **compiler and disassembly correlation:** Kavramın amacını, güven sınırını, olası hatalarını ve hangi durumda kullanılacağını açıklayın.

## Uygulama
Lab klasöründe `threads.c` dosyasını inceleyin ve şu komutları sırasıyla uygulayın:

```bash
gcc -g -O0 -pthread threads.c -o threads
```

```bash
./threads
```

```bash
gdb -q ./threads
```

```bash
readelf -sW threads | grep -E 'TLS|worker'
```

**Beklenen sonuç:** Each worker prints local=3; shared_total=6, but thread print order may vary.

## Analiz defteri
Kullandığınız araç sürümünü, komutu, girdiyi, gördüğünüz çıktıyı, beklenmeyen olayları ve olası açıklamaları kaydedin. Ölçülen veri ile çıkarımı ayrı başlıklar altında yazın.

## Uygulama görevleri
1. Orijinal örneği çalıştırıp çıktıyı kaydedin.
2. Zararsız bir girdiyi değiştirerek sonucu önceden tahmin edin.
3. Bir negatif test ekleyin ve gerçekten hangi kontrolün başarısız olduğunu belirleyin.
4. Başlangıç dosyasına dönüp regresyon testi yapın.
5. İki olası yanlış pozitif açıklaması üretin.
6. Sonucu doğrulayacak bağımsız bir kanıt kaynağı önerin.
7. Belirsizlikleri açıkça yazan kısa bir rapor hazırlayın.

## Tekrar soruları
1. pthread_create and pthread_join nedir, ne işe yarar, neyi garanti etmez?
2. thread-local storage nedir, ne işe yarar, neyi garanti etmez?
3. shared versus thread-local memory nedir, ne işe yarar, neyi garanti etmez?
4. synchronization and data races nedir, ne işe yarar, neyi garanti etmez?
5. GDB info threads nedir, ne işe yarar, neyi garanti etmez?
6. compiler and disassembly correlation nedir, ne işe yarar, neyi garanti etmez?
7. Sonucun güvenilirliğini nasıl artırırsınız?
8. Hangi kontrolü otomatik regresyon testine dönüştürürsünüz?

## Kapsam
Bu lab yalnızca izinli, sentetik veriler içindir. Tek bir bulguyu kesin saldırı kanıtı saymayın.
