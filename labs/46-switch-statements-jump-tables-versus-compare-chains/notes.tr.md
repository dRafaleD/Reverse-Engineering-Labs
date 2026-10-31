# Gün 46 — Switch İfadeleri: Atlama Tabloları ve Karşılaştırma Zincirleri

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Switch İfadeleri: Atlama Tabloları ve Karşılaştırma Zincirleri** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. Yoğun ve küçük değerler üzerindeki bir switch çoğunlukla bir tablo üzerinden dolaylı atlamaya derlenir.
2. Seyrek değerler üzerindeki bir switch genellikle karşılaştırma ve koşullu atlama zinciridir.
3. Dolaylı atlamadan önceki sınır kontrolü (`cmp` ve `ja`) tablo tabanlı switch'in imzasıdır.

## Kavramsal ayrımlar
- Formu derleyici seçer; yalnızca kaynak stili belirlemez.
- Tablolar salt okunur veride bulunur; hedefleri görmek için atlamayı izle.
- Decompiler'lar iki formu da switch olarak gösterebilir.

## Lab uygulaması
Önce `switch_forms.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O2 switch_forms.c -o switch_forms
~~~

~~~bash
./switch_forms
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn switch_forms | awk '/<dense>:/,/ret/'
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn switch_forms | awk '/<sparse>:/,/ret/'
~~~


## Görevler
1. Hangi fonksiyonun atlama tablosu kullanacağını tahmin et.
2. Derle ve yoğun fonksiyondaki dolaylı `jmp` komutunu bul.
3. Değerleri daha yoğun veya seyrek yap ve formun ne zaman değiştiğine bak.
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
