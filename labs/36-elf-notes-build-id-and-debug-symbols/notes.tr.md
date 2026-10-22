# Gün 36 — ELF Build ID, Debug Sembolleri ve Tekrarlanabilir Analiz

[English](notes.md) · [Türkçe](notes.tr.md)

## Hedef
ELF Build ID, Debug Sembolleri ve Tekrarlanabilir Analiz konusunu yerel, zararsız bir eğitim ortamında incele. Gözlenen veriyi yorumdan ayır ve sınırlamaları yaz.

## Kavramsal harita
1. Kontrol edilen girdi ve güven sınırı
2. Statik beklenti ve çalışma zamanı davranışı
3. Pozitif ve negatif testlerin farklı sonuçları
4. Yanlış pozitif, yanlış negatif ve belirsizlik
5. Savunma odaklı yorum ve doğrulama

## Uygulama
`symbols.c` dosyasını oku; ardından lab klasöründe çalıştır:

~~~bash
gcc -g -O0 -Wl,--build-id symbols.c -o symbols
~~~

~~~bash
readelf -n symbols
~~~

~~~bash
readelf -S symbols | grep debug
~~~

~~~bash
objcopy --only-keep-debug symbols symbols.debug
~~~

~~~bash
strip --strip-debug symbols
~~~

~~~bash
gdb -q -ex 'symbol-file symbols.debug' -ex 'quit' ./symbols
~~~

## Çalışma görevleri
1. Kodu çalıştırmadan beklediğin çıktıyı yaz.
2. Gerçek çıktıyı ve araç sürümünü kaydet.
3. Tek bir zararsız girdiyi değiştirip farkı açıkla.
4. Negatif bir test ekle.
5. Sonucun neyi kanıtlamadığını belirt.
6. Kaynak, komut, tarih, bulgu ve sınırlamalarla kısa rapor oluştur.

## Mini görev
Kritik beklentiyi doğrulayan bir assertion ekle ve bir başarısızlık durumunu test et.

## Tekrar soruları
1. Güven sınırı nerede?
2. Hangi değer sistemden geliyor?
3. Hangi girdiler doğrulanmalı?
4. Tek bir eşleşme ya da bulgu neden yeterli değildir?
5. Gerçek uygulamada hangi ek önlemleri alırsın?

## Etik
Yalnızca yerel veya açık izin verilmiş ortamlarda çalış.
