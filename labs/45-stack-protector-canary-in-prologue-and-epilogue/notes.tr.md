# Gün 45 — Prologue ve Epilogue'da Stack Protector Canary'si

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Prologue ve Epilogue'da Stack Protector Canary'si** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. `-fstack-protector-strong` ile yerel tamponu olan bir fonksiyon prologue'da canary'yi `fs:0x28` adresinden okuyup stack'e yazar.
2. Dönmeden önce epilogue saklanan değeri özgün değerle karşılaştırır ve farklıysa `__stack_chk_fail` çağırır.
3. Bu deseni görmek ikilinin stack koruması ile derlendiğini söyler ve yerel tamponu bulmaya da yardım eder.

## Kavramsal ayrımlar
- Canary bazı üzerine yazmaları dönüş anında yakalar; tüm bellek hatalarını önlemez.
- Bir fonksiyonun canary alıp almadığı derleyici bayraklarına ve yerel değişkenlerine bağlıdır.
- Bu lab yalnızca kodu okur; hiçbir taşmayı tetiklemez.

## Lab uygulaması
Önce `canary.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 -fstack-protector-strong canary.c -o canary
~~~

~~~bash
./canary
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn canary | grep -E 'fs:0x28|__stack_chk_fail'
~~~


## Görevler
1. `-fno-stack-protector` ile canary komutlarının görünüp görünmeyeceğini tahmin et.
2. `-fstack-protector-strong` ile derle ve `fs:0x28` erişimlerini bul.
3. Koruyucusuz yeniden derle ve iki disassembly'yi karşılaştır.
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
