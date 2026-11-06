# Gün 51 — String Literal'ları ve .rodata Bölümü

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **String Literal'ları ve .rodata Bölümü** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. String literal'ları salt okunur bir veri bölümünde, genellikle `.rodata`'da saklanır ve kod onlara adresle başvurur.
2. Disassembly'de `.rodata` içindeki bir adrese `lea` yapıp ardından `puts` veya `printf` çağırmak bir literal'in tipik kullanım şeklidir.
3. Bölümü dökmek her literal'i gösterir; bu, incelenecek ilginç kodu bulmanın hızlı bir yoludur.

## Kavramsal ayrımlar
- Literal'lar paylaşılır ve tekilleştirilir; adresler tekrar edebilir.
- Strip edilmiş bir ikili literal'lerini korur; strings bu yüzden yararlı kalır.
- Çalışma zamanında bir literal'e yazmak bir hatadır ve genellikle çökmeye yol açar.

## Lab uygulaması
Önce `rodata.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 rodata.c -o rodata
~~~

~~~bash
./rodata
~~~

~~~bash
objdump -s -j .rodata rodata
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn rodata | awk '/<main>:/,/ret/'
~~~


## Görevler
1. Program çıktısını tahmin et.
2. `.rodata` bölümünü dök ve iki literal'i bul.
3. `main` içinde literal'lerden birini yükleyen `lea` komutunu bul.
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
