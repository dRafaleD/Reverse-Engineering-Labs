# Gün 38 — GDB ile Stack Frame ve Yerel Değişken Yerleşimi

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **GDB ile Stack Frame ve Yerel Değişken Yerleşimi** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. Her çağrı; dönüş adresini, kayıtlı frame pointer'ı ve yerel değişkenleri tutan bir frame oluşturur.
2. Yerel değişkenler fonksiyon girişinde başlatılmamıştır; atamadan önce görülen değerler anlamlı değildir.
3. Frame yerleşimi derleyiciye, bayraklara ve optimizasyon seviyesine bağlıdır.

## Kavramsal ayrımlar
- Bir verinin bulunması, o veriye dayanarak kurulan bütün iddiaları doğrulamaz.
- Önce güven sınırını ve hangi girdiye kimin hükmettiğini belirle.
- Pozitif ve negatif kontrolleri birlikte çalıştır; bir ölçümün sınırlarını kaydet.

## Lab uygulaması
Önce `frames.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 -fno-omit-frame-pointer frames.c -o frames
~~~

~~~bash
./frames
~~~

~~~bash
gdb -q -batch -ex 'break sum_locals' -ex run -ex 'info frame' -ex 'info locals' -ex finish ./frames
~~~

## Görevler
1. Kodu inceleyerek beklediğin çıktıyı tahmin et.
2. Çalıştırıp girdileri ve gerçek çıktıları not et.
3. Zararsız bir girdiyi değiştir; farkı ve sebebini açıkla.
4. İlk güvenlik özelliği için assertion ekle.
5. Bir negatif senaryo ve bir yanlış pozitif ihtimali yaz.
6. İkinci bir bağımsız doğrulama yöntemi öner.

## Kısa sınav
1. Bu örnekte giriş verisi nereden geliyor?
2. Hangi davranış doğrudan gözlendi?
3. Hangi varsayımlar gerçek sistemde geçerli olmayabilir?
4. Hangi kanıt yetersiz kalır?
5. Bir otomatik test neyi kontrol etmeli?

## Etik ve sınırlamalar
Yalnızca izinli sistemlerde ve sentetik verilerle çalış. Bu örnek tam bir tespit veya koruma ürünü değildir.
