# Gün 42 — Fonksiyon İşaretçileri ve Dolaylı Çağrılar

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Fonksiyon İşaretçileri ve Dolaylı Çağrılar** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. Fonksiyon işaretçisi üzerinden çağrı, `call <isim>` yerine `call reg` olarak görünür.
2. Hedef yalnızca çalışma zamanında bilinir, bu yüzden statik disassembly onu adlandıramaz.
3. Hedefleri çözmek için çağrıya debugger breakpoint'i koy veya işaretçi tablosunu incele.

## Kavramsal ayrımlar
- Bir verinin bulunması, o veriye dayanarak kurulan bütün iddiaları doğrulamaz.
- Önce güven sınırını ve hangi girdiye kimin hükmettiğini belirle.
- Pozitif ve negatif kontrolleri birlikte çalıştır; bir ölçümün sınırlarını kaydet.

## Lab uygulaması
Önce `indirect.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 indirect.c -o indirect
~~~

~~~bash
./indirect
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn indirect | awk '/<apply>:/,/ret/'
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
