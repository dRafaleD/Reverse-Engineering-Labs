# Gün 37 — Çağrı Kuralları ve GDB ile Argüman İzleme

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Çağrı Kuralları ve GDB ile Argüman İzleme** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. On typical x86-64 System V ABI, the first integer arguments use RDI, RSI and RDX.
2. At -O0, debugger variable inspection is usually more straightforward than with optimization.
3. Debug symbols improve names but do not independently establish source-code integrity.

## Kavramsal ayrımlar
- Bir verinin bulunması, o veriye dayanarak kurulan bütün iddiaları doğrulamaz.
- Önce güven sınırını ve hangi girdiye kimin hükmettiğini belirle.
- Pozitif ve negatif kontrolleri birlikte çalıştır; bir ölçümün sınırlarını kaydet.

## Lab uygulaması
Önce `calls.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 -fno-omit-frame-pointer calls.c -o calls
~~~

~~~bash
./calls
~~~

~~~bash
gdb -q -ex 'break calculate' -ex run -ex 'info args' -ex 'bt' -ex quit ./calls
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
