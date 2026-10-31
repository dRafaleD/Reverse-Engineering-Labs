# Gün 49 — Tail Call'lar: call Yerine jmp

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Tail Call'lar: call Yerine jmp** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. Bir fonksiyonun son eylemi başka bir çağrının sonucunu döndürmekse, optimizer `call` ve `ret` yerine tek bir `jmp` koyabilir.
2. Çağrılan fonksiyon doğrudan özgün çağırana döner; böylece aradaki fonksiyon geri izlemeden kaybolur.
3. Bir fonksiyonun sonundaki bir fonksiyon adına `jmp` tanınabilir işarettir.

## Kavramsal ayrımlar
- `-O0`'da aynı kod normal `call` kullanır; iki derlemeyi karşılaştır.
- Geri izlemede eksik çerçeveler yalnızca bozulmadan değil tail call'lardan da gelebilir.
- Çağrıya adım adım girerek debugger'da doğrula.

## Lab uygulaması
Önce `tailcall.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O2 tailcall.c -o tailcall
~~~

~~~bash
./tailcall
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn tailcall | awk '/<outer>:/,/ret|jmp/'
~~~


## Görevler
1. Program çıktısını tahmin et.
2. `-O2` ile derle ve `outer` içindeki `jmp` komutunu bul.
3. `-O0` ile derle ve `call` formuyla karşılaştır.
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
