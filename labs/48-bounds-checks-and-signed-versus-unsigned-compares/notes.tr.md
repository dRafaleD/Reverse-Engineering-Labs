# Gün 48 — Sınır Kontrolleri ve İşaretli ile İşaretsiz Karşılaştırmalar

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Sınır Kontrolleri ve İşaretli ile İşaretsiz Karşılaştırmalar** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. İşaretli karşılaştırma `jl`/`jg`, işaretsiz karşılaştırma `jb`/`ja` kullanır; atlama seçimi değişkenin türünü ortaya çıkarır.
2. İşaretli indeks üzerinde `i < n` olarak yazılan sınır kontrolü negatif değerleri kabul eder; bunlar diziden önceki belleği indeksler.
3. `i >= 0 && i < n` kontrolü veya işaretsiz indeks kullanmak boşluğu kapatır.

## Kavramsal ayrımlar
- Bu lab değerleri yalnızca güvenli okur; negatif sayıyla asla indekslemez.
- Atlama komutu türün kanıtıdır, kaynağın kanıtı değil.
- Statik analiz araçları bu karşılaştırmaları işaretler, ama kodu okuyarak doğrula.

## Lab uygulaması
Önce `bounds.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 bounds.c -o bounds
~~~

~~~bash
./bounds
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn bounds | awk '/<weak_get>:/,/ret/'
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn bounds | awk '/<strict_get>:/,/ret/'
~~~


## Görevler
1. İki fonksiyonun 2 indeksi için çıktısını tahmin et.
2. Derle ve her fonksiyondaki atlama komutunu belirle.
3. `weak_get(a, -1, 4)` çağrısının neyi okuyacağını çalıştırmadan sözle anlat.
4. Bir güvenlik özelliği için assertion ekle.
5. Bir negatif senaryo ve bir yanlış pozitif ihtimali yaz.
6. Bağımsız bir doğrulama yöntemi öner.

## Kısa sınav
1. Girdi nereden geliyor ve onu kim kontrol ediyor?
2. Doğrudan gözlenen ile çıkarım yapılan arasındaki fark nedir?
3. Hangi varsayımlar gerçek sistemde geçerli olmayabilir?
4. Hangi kanıt tek başına yetersiz kalır?
5. Bir otomatik test neyi kontrol etmeli?

## Etik ve sınırlamalar
Yalnızca izinli sistemlerde ve sentetik verilerle çalış. Bu örnek tam bir tespit veya koruma ürünü değildir.
