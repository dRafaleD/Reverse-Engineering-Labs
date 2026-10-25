# Gün 39 — Disassembly'de Koşullar ve Dallanmalar

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Disassembly'de Koşullar ve Dallanmalar** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. Bir if ifadesi genellikle cmp ve ardından jns veya jne gibi koşullu bir atlamaya dönüşür.
2. Atlama çoğu zaman kaynak koşulunun tersini test eder: koşul yanlışsa gövdeyi atlar.
3. Disassembly derleyici çıktısıdır; aynı kaynak başka optimizasyon seviyelerinde farklı dallanmalar üretebilir.

## Kavramsal ayrımlar
- Bir verinin bulunması, o veriye dayanarak kurulan bütün iddiaları doğrulamaz.
- Önce güven sınırını ve hangi girdiye kimin hükmettiğini belirle.
- Pozitif ve negatif kontrolleri birlikte çalıştır; bir ölçümün sınırlarını kaydet.

## Lab uygulaması
Önce `branches.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 branches.c -o branches
~~~

~~~bash
./branches
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn branches | awk '/<classify>:/,/ret/'
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
