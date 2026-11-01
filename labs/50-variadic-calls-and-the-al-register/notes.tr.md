# Gün 50 — Variadic Çağrılar ve AL Register'ı

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Variadic Çağrılar ve AL Register'ı** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. x86-64 System V'de printf gibi variadic bir çağrı, AL'yi kullanılan vektör register sayısına ayarlar.
2. Bu yüzden `call printf` öncesinde `mov eax, 0` görünür.
3. Bu bir ABI ayrıntısıdır; programın verinden hesapladığı bir değer değildir.

## Kavramsal ayrımlar
- Bir verinin bulunması, o veriye dayanarak kurulan bütün iddiaları doğrulamaz.
- Önce güven sınırını ve hangi girdiye kimin hükmettiğini belirle.
- Pozitif ve negatif kontrolleri birlikte çalıştır; bir ölçümün sınırlarını kaydet.

## Lab uygulaması
Önce `variadic.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 variadic.c -o variadic
~~~

~~~bash
./variadic
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn variadic | awk '/<show>:/,/ret/'
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
