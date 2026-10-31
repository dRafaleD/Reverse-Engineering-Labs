# Gün 44 — Disassembly'de String Karşılaştırma Döngüleri

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Disassembly'de String Karşılaştırma Döngüleri** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. Elle yazılmış bir string karşılaştırması genellikle her iki string'den birer bayt yükleyip karşılaştıran bir döngü olarak görünür.
2. Döngü ilk farkta veya sonlandırıcı sıfır baytta çıkar; iki çıkış da koşullu atlamadır.
3. Derleyici döngüyü `strcmp` gibi bir kütüphane çağrısıyla değiştirebilir; bu yüzden çağrı tablosuna da bak.

## Kavramsal ayrımlar
- Bayt yüklemeleri (`movzx`) karşılaştırmanın kelime değil karakter bazlı olduğunu gösterir.
- Erken çıkış zamanlamayı sızdırır, sırlarda bu önemlidir; bu lab öğrenme amaçlı gösterir.
- `-O0` ve `-O2` çıktısını karşılaştır.

## Lab uygulaması
Önce `streq.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 streq.c -o streq
~~~

~~~bash
./streq
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn streq | awk '/<same>:/,/ret/'
~~~


## Görevler
1. Program çıktısını tahmin et.
2. Derle ve döngüden çıkan iki koşullu atlamayı bul.
3. `-O2` ile yeniden derle ve neyin değiştiğini anlat.
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
