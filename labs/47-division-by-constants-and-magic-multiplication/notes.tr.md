# Gün 47 — Sabitlere Bölme ve Sihirli Çarpma

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Sabitlere Bölme ve Sihirli Çarpma** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. Optimizasyon yapan derleyiciler sabitle bölmeyi sihirli bir sayıyla çarpma ve kaydırma ile değiştirir.
2. İşaretsiz 10'a bölmede genellikle `0xCCCCCCCD` ile çarpma ve ardından sağa kaydırma görürsün.
3. Deseni tanımak zaman kazandırır: kaynak `/ 10` dese de `div` komutu kullanılmaz.

## Kavramsal ayrımlar
- `-O0`'da derleyici genellikle gerçek `div` üretir; sihirli form optimizasyonla görünür.
- Kesin sabit bölene ve işaretliliğe bağlıdır.
- Fonksiyonu bir debugger'da birkaç girdiyle test ederek doğrula.

## Lab uygulaması
Önce `divconst.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O2 divconst.c -o divconst
~~~

~~~bash
./divconst
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn divconst | awk '/<div10>:/,/ret/'
~~~


## Görevler
1. Program çıktısını tahmin et.
2. `-O2` ile derle ve sihirli sabitle çarpmayı bul.
3. `-O0` ile derle ve `div` formuyla karşılaştır.
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
